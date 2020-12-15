var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

var deckController = require('../controllers/deckController')

router.post('/api/deck/create', authMiddleware, [
  body('title').trim().escape().notEmpty(),
  body('description').trim().escape(),
  body('characters.*.char').trim().escape().custom(value => {
    if (value.length < 1) {
      return true
    } else if (value.match(/[\u3400-\u9FBF]/)) {
      if (value.length > 1) {
        throw new Error('Only 1 character allowed per card')
      } else {
        return true
      }
    } else {
      throw new Error('Only Chinese characters allowed')
    }
  }),
  body('isPublic').customSanitizer(value => {
    if (typeof value == 'boolean') return value
    return value == 'true'
  }),
], deckController.createDeck)
router.post('/api/deck/update', authMiddleware, [
  body('title').trim().escape().notEmpty(),
  body('description').trim().escape(),
  body('characters.*.char').trim().escape().custom(value => {
    if (value.length < 1) {
      return true
    } else if (value.match(/[\u3400-\u9FBF]/)) {
      if (value.length > 1) {
        throw new Error('Only 1 character allowed per card')
      } else {
        return true
      }
    } else {
      throw new Error('Only Chinese characters allowed')
    }
  }),
  body('isPublic').customSanitizer(value => {
    if (typeof value == 'boolean') return value
    return value == 'true'
  }),
], deckController.updateDeck)
router.post('/api/deck/delete', authMiddleware, deckController.deleteDeck)

router.post('/api/deck/srs', authMiddleware, deckController.srs)
router.post('/api/deck/quizzed', authMiddleware, deckController.quizzed)
router.post('/api/deck/practiced', authMiddleware, deckController.practiced)
router.post('/api/deck/learned', authMiddleware, deckController.learned)

router.post('/api/deck/createdDecks', authMiddleware, deckController.findCreatedDecks)
router.post('/api/deck/getUsedDecks', authMiddleware, deckController.getUsedDecks)
router.post('/api/deck/getDecksWithDueDates', authMiddleware, deckController.getDecksWithDueDates)
router.post('/api/deck/deck', authMiddleware, deckController.deck)
router.get('/api/deck/public', authMiddleware, deckController.publicDecks)
router.get('/api/deck/assigned', authMiddleware, deckController.getAssignedDecks)

module.exports = router

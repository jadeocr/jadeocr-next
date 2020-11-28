var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

var deckController = require('../controllers/deckController')

router.post('/api/deck/create', authMiddleware, [
  body('title').trim().escape(),
  body('description').trim().escape(),
  body('characters.*').trim().escape().custom(value => {
    if (value.length < 1) {
      throw new Error('All characters must have values')
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

router.post('/api/deck/quizzed', authMiddleware, deckController.quizzed)

router.get('/api/deck/decks', authMiddleware, deckController.findDecks)
router.get('/api/deck/public', authMiddleware, deckController.publicDecks)
router.get('/api/deck/assigned', authMiddleware, deckController.getAssignedDecks)

module.exports = router

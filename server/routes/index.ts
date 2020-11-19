var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')

const authMiddleware = (req, res, next) => {
  if (!req.isAuthenticated()) {
    res.status(401).send('You are not authenticated')
  } else {
    return next()
  }
}

/* GET home page. */
router.post('/', function(req, res, next) {
  res.send('hello world')
})

var ocrController = require('../controllers/ocrController')
router.post('/api/ocr', ocrController.post)

var userController = require('../controllers/userController')
router.post('/api/signup', [
  body('email').isString().notEmpty().trim().escape().isEmail(),
  body('firstName').isString().notEmpty().trim().escape(),
  body('lastName').isString().notEmpty().trim().escape(),
  body('password').isString().notEmpty().trim().escape(),
  body('confirmPassword').isString().notEmpty().trim().escape(),
], userController.signup)
router.post('/api/signin', passport.authenticate('local'), function(req, res, next) {
  res.send(req.user)
})
router.post('/api/signout', authMiddleware, function(req, res, next) {
  req.logout()
  res.sendStatus(200)
})
router.get('/api/user', authMiddleware, userController.user)
router.get('/api/user/details', authMiddleware, userController.details)

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
    if (typeof value == 'boolean') {
      return value
    } else if (value == 'true') {
      return true
    } else if (value == 'false') {
      return false
    } else {
      return false
    }
  }),
], deckController.createDeck)
router.get('/api/deck/decks', authMiddleware, deckController.findDecks)
router.get('/api/deck/public', authMiddleware, deckController.publicDecks)
router.get('/api/deck/assigned', authMiddleware, deckController.getAssignedDecks)

var characterController = require('../controllers/characterController')
router.get('/api/pinyin', characterController.pinyin)
router.get('/api/definition', characterController.definition)
router.get('/api/strokes', [
  body('character').trim().escape(),
  ], characterController.graphics)
router.get('/api/animated', [
  body('character').trim().escape(),
  ], characterController.animated)
router.get('/api/static', [
  body('character').trim().escape(),
  ], characterController.static)

var classController = require('../controllers/classController')
router.post('/api/class/create', authMiddleware, [
  body('className').trim().escape(),
  body('description').trim().escape(),
], classController.createClass)
router.post('/api/class/remove', authMiddleware, [
  body('classCode').trim().escape(),
], classController.removeClass)
router.post('/api/class/join', authMiddleware, [
  body('classCode').trim().escape(),
], classController.join)
router.post('/api/class/leave', authMiddleware, [
  body('classCode').trim().escape(),
], classController.leave)
router.post('/api/class/assign', authMiddleware, [
  body('classCode').trim().escape(),
  body('deck').trim().escape(),
], classController.assign)
router.post('/api/class/unassign', authMiddleware, [
  body('classCode').trim().escape(),
  body('deck').trim().escape(),
], classController.unassign)
router.post('/api/class/assigned', authMiddleware, [
  body('classCode').trim().escape(),
], classController.getAssignedDecks)

module.exports = router
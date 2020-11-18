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
router.post('/api/ocr', cors(), ocrController.post)

var userController = require('../controllers/userController')
router.post('/api/signup', cors(), [
  body('email').trim().escape(),
  body('firstName').trim().escape(),
  body('lastName').trim().escape(),
  body('password').trim().escape(),
  body('confirmPassword').trim().escape(),
], userController.signup)
router.post('/api/signin', cors(), passport.authenticate('local'), function(req, res, next) {
  res.send(req.user)
})
router.post('/api/signout', cors(), authMiddleware, function(req, res, next) {
  req.logout()
  res.sendStatus(200)
})
router.get('/api/user', cors(), authMiddleware, userController.user)
router.get('/api/user/details', cors(), authMiddleware, userController.details)

var deckController = require('../controllers/deckController')
router.post('/api/deck/create', cors(), authMiddleware, [
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
router.get('/api/deck/decks', cors(), authMiddleware, deckController.findDecks)
router.get('/api/deck/public', cors(), authMiddleware, deckController.publicDecks)

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
router.post('/api/class/create', cors(), authMiddleware, [
  body('className').trim().escape(),
  body('description').trim().escape(),
], classController.createClass)
router.post('/api/class/remove', cors(), authMiddleware, [
  body('classCode').trim().escape(),
], classController.removeClass)
router.post('/api/class/join', cors(), authMiddleware, [
  body('classCode').trim().escape(),
], classController.join)
router.post('/api/class/leave', cors(), authMiddleware, [
  body('classCode').trim().escape(),
], classController.leave)
router.post('/api/class/assign', cors(), authMiddleware, [
  body('classCode').trim().escape(),
  body('deck').trim().escape(),
], classController.assign)

module.exports = router
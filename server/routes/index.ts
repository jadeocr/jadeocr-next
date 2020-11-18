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

var deckController = require('../controllers/deckController')
router.post('/api/create/deck', cors(), authMiddleware, deckController.createDeck)
router.post('/api/decks', cors(), authMiddleware, deckController.findDecks)
router.post('/api/publicdecks', cors(), authMiddleware, deckController.publicDecks)

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
router.post('/api/class/join', cors(), authMiddleware, [
  body('classCode').trim().escape()
  ], classController.join)

module.exports = router
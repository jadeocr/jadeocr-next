var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')

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

var newUserController = require('../controllers/newUserController')
router.post('/api/signup', cors(), newUserController.post)
router.post('/api/signin', cors(), passport.authenticate('local'), function(req, res, next) {
  res.sendStatus(200)
})

router.post('/api/decks', cors(), function(req, res, next) {
  res.send(["one", "two", "three"])
})

var deckController = require('../controllers/deckController')
router.post('/api/createdeck', cors(), authMiddleware, deckController.createDeck)

var characterController = require('../controllers/characterController')
router.get('/api/pinyin', characterController.pinyin)
router.get('/api/definition', characterController.definition)

module.exports = router


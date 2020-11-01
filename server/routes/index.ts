var express = require('express')
var router = express.Router()
const cors = require('cors')

/* GET home page. */
router.post('/', function(req, res, next) {
  res.send('hello world')
})

var ocrController = require('../controllers/ocrController')
router.post('/api/ocr', cors(), ocrController.post)

var newUserController = require('../controllers/newUserController')
router.post('/api/signup', cors(), newUserController.post)

router.post('/api/decks', cors(), function(req, res, next) {
  res.send(["one", "two", "three"])
})

module.exports = router


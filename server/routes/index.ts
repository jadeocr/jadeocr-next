var express = require('express')
var router = express.Router()

/* GET home page. */
router.get('/', function(req, res, next) {
  res.send('hello world')
})

var ocrController = require('../controllers/ocrController')
router.post('/api/ocr', ocrController.post)

var newUserController = require('../controllers/newUserController')
router.post('/api/signup', newUserController.post)

module.exports = router

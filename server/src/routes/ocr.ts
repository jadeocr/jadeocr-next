var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

var ocrController = require('../controllers/ocrController')

router.post('/api/ocr', ocrController.ocr)

module.exports = router

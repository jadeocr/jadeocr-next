var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

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

module.exports = router

var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

var characterController = require('../controllers/characterController')

router.post('/api/pinyin', characterController.pinyin)
router.post('/api/definition', characterController.definition)
router.post('/api/pinyinAndDefinition', characterController.pinyinAndDefinition)

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

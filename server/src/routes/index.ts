var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

// Get home page
router.post('/', function(req, res, next) {
  res.send('hello world')
})

module.exports = router


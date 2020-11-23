var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

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

module.exports = router

var express = require('express')
var router = express.Router()
var cors = require('cors')
var passport = require('passport')
var { body } = require('express-validator')
var { authMiddleware } = require('./authMiddleware')

var classController = require('../controllers/classController')

router.post('/api/class/create', authMiddleware, [
  body('className').trim().escape(),
  body('description').trim().escape(),
], classController.createClass)

router.post('/api/class/remove', authMiddleware, [
  body('classCode').trim().escape(),
], classController.removeClass)

router.post('/api/class/join', authMiddleware, [
  body('classCode').trim().escape(),
], classController.join)

router.post('/api/class/leave', authMiddleware, [
  body('classCode').trim().escape(),
], classController.leave)

router.post('/api/class/getJoinedClasses', authMiddleware, classController.getJoinedClasses)
router.post('/api/class/getTeachingClasses', authMiddleware, classController.getTeachingClasses)

router.post('/api/class/class', authMiddleware, [
  body('classCode').trim().escape(),
], classController.Class)

router.post('/api/class/assign', authMiddleware, [
  body('classCode').trim().escape(),
  body('deck').trim().escape(),
], classController.assign)

router.post('/api/class/unassign', authMiddleware, [
  body('classCode').trim().escape(),
  body('deck').trim().escape(),
], classController.unassign)

router.post('/api/class/assigned', authMiddleware, [
  body('classCode').trim().escape(),
], classController.getAssignedDecks)

module.exports = router

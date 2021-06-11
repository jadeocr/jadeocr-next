import e = require("express")

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
  body('classCode').trim().escape().exists(),
  body('deckId').trim().escape().exists(),
  body('mode').custom(value => {
    if ((value == "learn") || (value == "quiz") || (value == "srs")) {
      return true
    } else {
      throw new Error('mode must be: "learn", "srs", "quiz"')
    }
  }),
  body("handwriting").trim().toBoolean(),
  body("front").custom(value => {
    if ((value == "character") || (value == "pinyin") || (value == "definition") || (value == "handwriting")) {
      return true
    } else {
      throw new Error('front must be: "character", "pinyin", "definition", "handwriting"')
    }
  }),
  body("scramble").trim().toBoolean(),
  body("repetitions").trim().escape(),
  body("dueDate").trim().exists().isNumeric(),
], classController.assign)

router.post('/api/class/updateAssignment', authMiddleware, [
  body('classCode').trim().escape().exists(),
  body("handwriting").trim().toBoolean(),
  body("front").custom(value => {
    if ((value == "character") || (value == "pinyin") || (value == "definition") || (value == "handwriting")) {
      return true
    } else {
      throw new Error('front must be: "character", "pinyin", "definition", "handwriting"')
    }
  }),
  body("scramble").trim().toBoolean(),
  body("dueDate").trim().exists().isNumeric(),
], classController.updateAssignment)

router.post('/api/class/unassign', authMiddleware, [
  body('classCode').trim().escape(),
  body('deckId').trim().escape(),
  body('assignmentId').trim().escape(),
], classController.unassign)

router.post('/api/class/getAssignedDecksAsStudent', authMiddleware, [
  body('classCode').trim().escape(),
], classController.getAssignedDecksAsStudent)

router.post('/api/class/getAssignedDecksAsTeacher', authMiddleware, [
  body('classCode').trim().escape(),
], classController.getAssignedDecksAsTeacher)

router.post('/api/class/submitFinishedDeck', authMiddleware, classController.submitFinishedDeck)

router.post('/api/class/getDeckResults', authMiddleware, [
  body('classCode').trim().escape(),
  body('deckId').trim().escape(),
], classController.getDeckResults)

module.exports = router

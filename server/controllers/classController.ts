var classModel = require('../models/classModel')

exports.createClass = function(req, res, next) {
    if (req.user.isTeacher == false) {
        res.sendStatus(403)
    } else {
        let className = 
    }
}
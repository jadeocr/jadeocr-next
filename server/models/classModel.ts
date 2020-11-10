var mongoose = require('mongoose')

var classSchema = new mongoose.Schema({
    teacher: String,
    title: String,
    description: String,
    students: Array,
})

module.exports = new mongoose.model('classModel', classSchema)
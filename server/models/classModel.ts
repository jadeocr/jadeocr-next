var mongoose = require('mongoose')

var classSchema = new mongoose.Schema({
    teacher: String,
    name: String,
    description: String,
    students: Array,
    invites: Array,
    private: Boolean,
    assignedDecks: Array,
})

module.exports = new mongoose.model('classModel', classSchema)
var mongoose = require('mongoose')

var userDetailedSchema = new mongoose.Schema({
    email: String,
    password: String,
    isTeacher: Boolean,
    classes: Array,
    stats: Array,
})

module.exports = mongoose.model('userDetailed', userDetailedSchema)
var mongoose = require('mongoose')

var userDetailedSchema = new mongoose.Schema({
    id: String,
    email: String,
    firstName: String,
    lastName: String,
    password: String,
    isTeacher: Boolean,
    classes: Array,
    stats: Array,
})

module.exports = mongoose.model('userDetailed', userDetailedSchema)
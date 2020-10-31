const { mongo } = require('mongoose')
var mongoose = require('mongoose')

var userSchema = new mongoose.Schema({
    username: String,
    password: String,
    isTeacher: Boolean,
})

module.exports = mongoose.model("userModel", userSchema)
var mongoose = require('mongoose')

var userSchema = new mongoose.Schema({
  email: String,
  password: String,
  isTeacher: Boolean,
})

module.exports = mongoose.model('users', userSchema)

var mongoose = require('mongoose')

var userSchema = new mongoose.Schema({
  email: String,
  firstName: String,
  lastName: String,
  password: String,
  isTeacher: Boolean,
})

module.exports = mongoose.model('users', userSchema)

var userModel = require('../models/userModel')
var bcrypt = require('bcryptjs')

exports.post = function(req, res, next) {
  let email = req.body.email
  let password = req.body.password
  let confirmPassword = req.body.confirmPassword
  let isTeacher = false

  userModel.findOne({ email: email }, function(err, found) {
    if (found) {
      res.sendStatus(400)
    } else if (!(email && password && confirmPassword)) {
      res.sendStatus(400)
    } else if (password != confirmPassword) {
      res.sendStatus(400)
    } else {
      bcrypt.hash(password, 10, function(err, hash) {
        if (err) console.log(err)
        var newUser = new userModel({
          email: email,
          password: hash,
          isTeacher: isTeacher,
        })
        newUser.save(function(err) {
          if (err) console.log(err)
          res.sendStatus(200)
        })
      })
    }
  })
}

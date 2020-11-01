var userModel = require('../models/userModel')
var bcrypt = require('bcryptjs')

exports.post = function(req, res, next) {
  var user = 'placeholder'
  var password = 'placeholder'
  var passwordAgain = 'placeholder'
  var isTeacher = false

  userModel.find({username: user}, function(err, found) {
    if (found) {
      res.sendStatus(400)
    } else if (!(user && password && passwordAgain)) {
      res.sendStatus(400)
    } else if (password != passwordAgain) {
      res.sendStatus(400)
    } else {
      bcrypt.hash(password, 10, function(err, hash) {
        if (err) console.log(err)
        var newUser = new userModel({
          username: user,
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

var userModel = require('../models/userModel')
var userDetailedModel = require('../models/userDetailedModel')
var bcrypt = require('bcryptjs')

exports.signup = function(req, res, next) {
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
          userModel.findOne({email: email}, function(err, user) {
            let detailedUser = new userDetailedModel({
              id: String(user._id),
              email: user.email,
              password: user.password,
              isTeacher: user.isTeacher,
            })
            detailedUser.save(function(err) {
              if (err) console.log(err)
            res.sendStatus(200)
            })
          })
        })
      })
    }
  })
}

exports.user = function(req, res, next) {
  res.send(req.user)
}

exports.details = function(req, res, next) {
  userDetailedModel.findOne({id: req.user._id}, function(err, user) {
    if (err) {
      console.log(err)
      res.send('There was an error')
    } else {
      res.send(user)
    }
  })
}
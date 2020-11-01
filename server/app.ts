var bcrypt = require('bcryptjs')
var cors = require('cors')

var createError = require('http-errors')
var express = require('express')
var path = require('path')
var cookieParser = require('cookie-parser')
var logger = require('morgan')

var indexRouter = require('./routes/index')

var mongoose = require('mongoose')
var env = require('./env')
mongoose.connect(env.mongooseURL, {
  useNewUrlParser: true,
  useUnifiedTopology: true,
  useFindAndModify: false,
  useCreateIndex: true,
})


var app = express()

var passport = require('passport')
var userModel = require('./models/userModel')
var LocalStrategy = require('passport-local').Strategy
passport.use(new LocalStrategy({
    usernameField: 'email',
    passwordField: 'password',
  },
  function(username, password, done) {
    userModel.findOne({ email: username }, function(err, found) {
      if (!found) {
        done(null, false)
      } else {
        bcrypt.compare(password, found.password, function(err, result) {
          if (result) {
            return done(null, found)
          } else {
            return done(null, false)
          }
        })
      }
    })  
  }
))

passport.serializeUser(function(user, done) {
  done(null, user._id)
})

passport.deserializeUser(function(id, done) {
  userModel.findOne({_id: id}, function(err, user) {
    if (err) console.log(err)
    done(null, user)
  })
})

app.use(passport.initialize())
app.use(passport.session())

let whitelist = ['http://localhost:8080']

app.use(cors({
  origin: function (origin, callback) {
    // allow requests with no origin 
    if (!origin) return callback(null, true);
    if (whitelist.indexOf(origin) === -1) {
      var message = 'The CORS policy for this origin doesn\'t ' +
      'allow access from the particular origin.';
      return callback(new Error(message), false);
    }
    return callback(null, true);
  }
}))

// view engine setup
app.set('views', path.join(__dirname, 'views'))
app.set('view engine', 'pug')

app.use(logger('dev'))
app.use(express.json())
app.use(express.urlencoded({ extended: false }))
app.use(cookieParser())
app.use(express.static(path.join(__dirname, 'public')))

app.use('/', indexRouter)

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404))
})

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message
  res.locals.error = req.app.get('env') === 'development' ? err : {}

  // render the error page
  res.status(err.status || 500)
  res.render('error')
})

module.exports = app

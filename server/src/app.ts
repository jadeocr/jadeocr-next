var bcrypt = require('bcryptjs')
var cors = require('cors')

var createError = require('http-errors')
var express = require('express')
var path = require('path')
var cookieParser = require('cookie-parser')
var logger = require('morgan')

var mongoose = require('mongoose')
const dotenv = require('dotenv').config()

mongoose.connect(process.env.MONGOOSEURL, {
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

var session = require('express-session');
var MongoStore = require('connect-mongo')(session);

app.use(session({
    secret: 'superSecretSecrets!!alsk;dfjiekmcjfueisldkfjur;alskdjfuiroijrgl;kjasdfhuhawiohrfga;sldkjfhwas;leikfjdass;ldkjfa;lskdjf',
    resave: false,
    saveUninitialized: false,
    store: new MongoStore({ mongooseConnection: mongoose.connection }),
}));

app.use(passport.initialize())
app.use(passport.session())

app.use(cors({
  origin: 'http://localhost:8080',
  credentials: true,
  exposedHeaders: ["set-cookie"],
}))

// view engine setup
app.set('views', path.join(__dirname, 'views'))
app.set('view engine', 'pug')

app.use(logger('dev'))
app.use(express.json())
app.use(express.urlencoded({ extended: false }))
app.use(cookieParser())
app.use(express.static(path.join(__dirname, 'public')))


var indexRouter = require('./routes/index')
var userRouter = require('./routes/user')
var decksRouter = require('./routes/deck')
var characterRouter = require('./routes/character')
var classRouter = require('./routes/class')
var ocrRouter = require('./routes/ocr')

app.use('/', indexRouter)
app.use('/', userRouter)
app.use('/', decksRouter)
app.use('/', characterRouter)
app.use('/', classRouter)
app.use('/', ocrRouter)

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

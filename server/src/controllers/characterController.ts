var characterModel = require('../models/characterModel')
var strokeModel = require('../models/strokeModel')
var animatedSVGModel = require('../models/animatedSVGModel')
var staticSVGModel = require('../models/staticSVGModel')

exports.pinyin = function(req, res, next) {
    let character = req.body.character

    characterModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.pinyin)
        }
    })
}

exports.definition = function(req, res, next) {
    let character = req.body.character

    characterModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.definition)
        }
    })
}

exports.pinyinAndDefinition = function(req, res, next) {
    let character = req.body.character

    characterModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send({
                definition: char.definition,
                pinyin: char.pinyin
            })
        }
    })
}

exports.graphics = function(req, res, next) {
    let character = req.body.character

    strokeModel.findOne({character: character}, function(err, strokes) {
        if (err) console.log(err)
        if (!strokes) {
            res.send("No matches found")
        } else {
            res.send(strokes.strokes)
        }
    })
}

exports.animated = function(req, res, next) {
    let character = req.body.character

    animatedSVGModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.svg)
        }
    })
}

exports.static = function(req, res, next) {
    let character = req.body.character

    staticSVGModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.svg)
        }
    })
}
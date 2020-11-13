var characterModel = require('../models/characterModel')
var strokeModel = require('../models/strokeModel')
var animatedSVGModel = require('../models/animatedSVGModel')

exports.pinyin = function(req, res, next) {
    let character = req.body.character

    characterModel.findOne({character: character}, function(err, char) {
        if (err) console.log(err)
        if (!char) {
            res.send("No matches found")
        } else {
            res.send(char.pinyin)
            console.log(char)
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

    animatedSVGModel.findOne({character: character}, function(err, svgs) {
        if (err) console.log(err)
        console.log(svgs.svg)
        res.send(svgs.svg)
    })
}
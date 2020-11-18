var deckModel = require('../models/deckModel')
var userDetailedModel = require('../models/userDetailedModel')
var classModel = require('../models/classModel')
var { validationResult } = require('express-validator')

exports.createDeck = function(req, res, next) {
    let errors = validationResult(req)
    
    if (!errors.isEmpty()) {
        res.send(errors)
    } else {
        var deck = new deckModel({
            title: req.body.title,
            description: req.body.description,
            characters: req.body.characters,
            creator: req.user.id,
            isPublic: req.body.isPublic
        })
        
        deck.save(function(err) {
            if (err) {
                console.log(err)
                res.sendStatus(400)
            } else {
                res.sendStatus(200)
            }
        })
    }    
}

exports.findDecks = function(req, res, next) {
    deckModel.find({creator: req.user._id}, function(err, decks) {
        if (err) console.log(err) 
        res.send(decks)
    })
}

exports.publicDecks = function(req, res, next) {
    deckModel.find({isPublic: true}, function(err, decks) {
        if (err) console.log(err) 
        res.send(decks)
    })
}

exports.getAssignedDecks = function(req, res, next) {
    let student = req.user.user

    userDetailedModel.findOne({email: student.email}, function(err, user) {
        if (err) {
            console.log(user)
            res.send("No user found. Probably errors within database.")
        } else {
            var assignedDecks = []

            for (let i of user.classes) {
                classModel.findOne({_id: i}, function(err, Class) {
                    if (err) {
                        console.log("err")
                        res.send("There was an error while finding classes for this user")
                    } else {
                        assignedDecks.push(Class.assignedDecks)
                        console.log('in classes')
                    }
                })
            }
            console.log("got out of classes")
            res.sendStatus(200)
        }
    })
}
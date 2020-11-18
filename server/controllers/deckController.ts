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
            creatorFirst: req.user.firstName,
            creatorLast: req.user.lastName,
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
    let user = String(req.user._id)

    userDetailedModel.findOne({id: user}, function(err, user) {
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (user.classes.length == 0) {
            res.status(400).send('User is not enrolled in any classes')
        } else {
            var classArray = []

            for (let i in user.classes) {
                classArray.push({classCode: user.classes[i]})

                if (parseInt(i) + 1 == user.classes.length) {
                    var deckArray = []

                    classModel.find({$or: classArray}, function(err, classResults) {
                        if (err) {
                            console.log(err)
                            res.status(400).send('There was an error')
                        } else {
                            for (let j in classResults) {
                                deckArray.push({_id: classResults[j].assignedDecks})

                                if (parseInt(j) + 1 == classResults.length) {
                                    deckModel.find({$or: deckArray}, function(err, deckResults) {
                                        if (err) {
                                            console.log(err)
                                            res.status(400).send('There was an error')
                                        } else {
                                            res.status(200).send(deckResults)
                                        }
                                    })
                                }
                            }
                        }
                    })
                }
            }
        }
    })
}
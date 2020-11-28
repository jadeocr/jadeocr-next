import { Decipher } from "crypto"
import e = require("express")
import { send } from "process"

var deckModel = require('../models/deckModel')
var userDetailedModel = require('../models/userDetailedModel')
var classModel = require('../models/classModel')
var { validationResult } = require('express-validator')
var {v4: uuidv4} = require('uuid')

exports.createDeck = function(req, res, next) {
    let errors = validationResult(req)

    if (!errors.isEmpty()) {
        res.send(errors)
    } else {
        let characterArray = []
        for (let i of req.body.characters) {
            characterArray.push({char: i, id: uuidv4()})
        }

        var deck = new deckModel({
            title: req.body.title,
            description: req.body.description,
            characters: characterArray,
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

exports.srs = function(req, res, next) {
    let deckId = req.body.deckId

    userDetailedModel.findOne({id: req.user._id}, function(err, user) {
        if (err) {
            console.log(err)
            req.status(400).send('There was an error')
        } else if (!user) {
            req.status(400).send('No user found')
        } else {
            deckModel.findOne({_id: deckId}, function(err, deck) {
                if (err) {
                    console.log(err)
                    req.status(400).send('There was an error')
                } else if (!deck) {
                    req.status(400).send('No deck found')
                } else {
                    let sendArray = []
                    if (user.decks.length == 0) {
                        sendArray.push({deck: deck, srs: false})
                        req.status(200).send(sendArray)
                        return
                    } else {
                        for (let i in user.decks) {
                            if (user.decks[i].deckId == deckId) {
                                sendArray.push({deck: deck, srs: user.decks[i].srs})
                                req.status(200).send(sendArray)
                                return
                            } else if (parseInt(i) + 1 == user.decks.length) {
                                sendArray.push({deck: deck, srs: false})
                                req.status(200).send(sendArray)
                                return
                            }
                        }
                    }
                }
            })
        }
    })
}

exports.publicDecks = function(req, res, next) {
    deckModel.find({isPublic: true}, function(err, decks) {
        if (err) console.log(err) 
        res.send(decks)
    })
}

exports.getAssignedDecks = function(req, res, next) {
    let userId = String(req.user._id)

    userDetailedModel.findOne({id: userId}, function(err, user) {
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

exports.practiced = function(req, res, next) {
    let deckId = req.body.deck
    let results = req.body.results
    let userId = String(req.user._id)
}

exports.quizzed = function(req, res, next) {
    let deckId = req.body.deck
    let results = req.body.results
    let userId = String(req.user._id)

    userDetailedModel.findOne({id: userId}, function(userErr, user) { //user_err might always evaluate to true tho /s
        //Doesn't use "_id" because "_id" autogenerated, linked to main user db with "id"
        if (userErr) console.log(userErr)

        if (!user) {
            res.status(400).send("Could not find user")
        } else if (!results) {
            res.status(400).send("No results sent")
        } else {
            deckModel.findOne({_id: deckId}, function(deck_err, deck) {
                if (deck_err) console.log(deck_err)

                if (!deck) {
                    res.status(400).send("Could not find deck")
                } else {
                    if (user.decks.length == 0) {
                        user.decks.push({deckId: deckId})
                        writeResultsToUser(results, user.decks[0], 0)
                        user.save(function(err) {
                            if (err) {
                                res.status(400).send(err)
                            } else {
                                res.sendStatus(200)
                            }
                        })
                        return
                    } else {
                        for (let i in user.decks) {
                            if (user.decks[i].deckId == deckId) {
                                console.log('got in')
                                writeResultsToUser(results, user.decks[i], user.decks[i].totalQuizAttempts)
                                user.save(function(err) {
                                    if (err) {
                                        res.status(400).send(err)
                                    } else {
                                        res.sendStatus(200)
                                    }
                                })
                                return
                            } else if (parseInt(i) + 1 == user.decks.length) {
                                user.decks.push({deckId: deckId})
                                writeResultsToUser(results, user.decks[parseInt(i) + 1], 0)
                                user.save(function(err) {
                                    if (err) {
                                        res.status(400).send(err)
                                    } else {
                                        res.sendStatus(200)
                                    }
                                })
                                return
                            }
                        }
                    }
                }
            })
        }
    })

    let writeResultsToUser = function(results, deck, attemptNumber) {
        let writeArray = []
        let numCorrect = 0
        let overriden = 0

        for (let i of results) {
            writeArray.push({charId: i.id, correct: i.correct, overriden: i.overriden})
            if (i.correct == true) {
                numCorrect++
            }
            if (i.overriden == true) {
                overriden++
            }
        }

        deck.totalQuizAttempts = attemptNumber + 1
        deck.quizAttempts.push({
            attempt: attemptNumber + 1,
            summary: {
                correct: numCorrect,
                total: results.length,
                overriden: overriden,
            },
            stats: writeArray
        })
    }
}
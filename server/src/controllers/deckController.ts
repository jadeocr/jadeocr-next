var deckModel = require('../models/deckModel')
var userDetailedModel = require('../models/userDetailedModel')
var classModel = require('../models/classModel')
var { validationResult } = require('express-validator')
var {v4: uuidv4} = require('uuid')

let intersect = function(a, b) {
    var setA = new Set(a);
    var setB = new Set(b);
    var intersection = new Set([...setA].filter(x => setB.has(x)));
    return Array.from(intersection);
  }

let checkAccess = function(user, deck, callback) {
    if (deck.creator == user.id) {
        callback(true, user, deck)
    } else if (deck.access.isPublic == true) {
        callback(true, user, deck)
    } else {
        if (intersect(user.classes, deck.access.classes).length > 0) {
            callback(true, user, deck)
        } else {
            callback(false, user, deck)
        }
    }
}

let updateLatestAccessDate = function(user, deck, callback) {
    
    let createNewDeckEntry = function(user, deck) {
        user.decks.push({
            deckId: deck._id,
            deckName: deck.title,
            deckDescription: deck.description,
            latestAccessDate: Date.now(),
        })
    }

    checkIfDeckEntryExists(user, deck._id, function(i) {
        if (i !== false) {
            user.decks[i].latestAccessDate = Date.now()
        } else {
            createNewDeckEntry(user, deck)
        }
    })

    user.save(function(saveUserErr) {
        if (saveUserErr) {
            console.log(saveUserErr)
            callback(false)
        } else {
            callback(true)
        }
    })
}

let checkIfDeckEntryExists = function(user, deckId, callback) {
    if (!user.decks.length) {
        callback(false)
    } else {
        for (let i in user.decks) {
            if (user.decks[i].deckId == deckId) {
                callback(i)
            } else if (parseInt(i) == user.decks.length - 1) {
                callback(false)
            }
        }
    }
}

let compare = function(a, b) {
    return a.nextDue - b.nextDue
}

exports.createDeck = function(req, res, next) {
    let errors = validationResult(req)
    let userId = String(req.user._id)

    if (!errors.isEmpty()) {
        res.status(400).send(errors)
    } else {
        let characterArray = []
        for (let i of req.body.characters) {
            if (!i.char) {
                continue
            } else {
                characterArray.push({
                    char: i.char,
                    definition: i.definition,
                    pinyin: i.pinyin,
                    id: uuidv4()
                })
            }
        }

        var deck = new deckModel({
            title: req.body.title,
            description: req.body.description,
            characters: characterArray,
            creator: req.user.id,
            creatorFirst: req.user.firstName,
            creatorLast: req.user.lastName,
            createdDate: Date.now(),
            access: {
                isPublic: req.body.isPublic
            }
        })
        
        deck.save(function(err, savedDeck) {
            if (err) {
                console.log(err)
                res.sendStatus(400)
            } else {
                userDetailedModel.findOne({id: userId}, function (userErr, user) {
                    user.decks.push({
                        deckId: savedDeck._id,
                        deckName: savedDeck.title,
                        deckDescription: savedDeck.description,
                        latestAccessDate: Date.now(),
                        isOwner: true,
                    })

                    updateLatestAccessDate(user, savedDeck, function(result) {
                        if (result) {
                            res.sendStatus(200)
                        } else {
                            res.sendStatus(400)
                        }
                    })
                })
            }
        })
    }    
}

exports.updateDeck = function(req, res, next) {
    let deckId = req.body.deckId
    let userId = String(req.user._id)
    let title = req.body.title
    let description = req.body.description
    let isPublic = req.body.isPublic
    let characters = req.body.characters

    let errors = validationResult(req)
    if (!errors.isEmpty()) {
        res.status(400).send(errors)
    } else {
        deckModel.findById(deckId, function(err, deck) {
            if (err) {
                console.log(err)
                res.status(400).send('There was an error')
            } else if (!deck) {
                res.status(400).send('No deck found')
            } else if (deck.creator == userId) {
                let characterArray = []
                for (let i of characters) {
                    let id = i.id || uuidv4()

                    if (!i.char) {
                        continue
                    } else {
                        characterArray.push({
                            char: i.char,
                            definition: i.definition,
                            pinyin: i.pinyin,
                            id: id
                        })
                    }
                }

                deck.title = title
                deck.description = description
                deck.characters = characterArray
                deck.access = {
                    isPublic: isPublic
                }

                deck.save(function(saveErr) {
                    if (saveErr) console.log(saveErr)
                    userDetailedModel.findOne({id: userId}, function(userErr, user) {
                        if (userErr) {
                            console.log(userErr)
                            res.status(400).send('There was an error')
                        } else if (!user) {
                            res.status(400).send('No user found')
                        } else {
                            updateLatestAccessDate(user, deck, function(result) {
                                if (result) {
                                    res.sendStatus(200)
                                } else {
                                    res.sendStatus(400)
                                }
                            })
                        }
                    })
                })
            } else {
                res.status(403).send('User does not have access to deck')
            }
        })
    }
}

exports.deleteDeck = function(req, res, next) {
    let deckId = req.body.deckId
    let userId = String(req.user._id)

    deckModel.findById(deckId, function(err, deck) {
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!deck) {
            res.status(400).send('No deck found')
        } else if (deck.creator == userId) {
            deck.remove(function(removeErr) {
                if (err) console.log(removeErr)
                res.send('Deck removed')
            })
        } else {
            res.status(403).send('User does not have access to deck')
        }
    })
}

exports.findCreatedDecks = function(req, res, next) {
    deckModel.find({creator: req.user._id}, function(err, decks) {
        if (err) console.log(err) 
        let sendArray = []
        for (let deck of decks) {
            sendArray.push({
                deckId: deck._id,
                deckName: deck.title,
                deckDescription: deck.description
            })
        }
        res.send(sendArray)
    })
}

exports.getUsedDecks = function(req, res, next) {
    userDetailedModel.findOne({id: req.user._id}, function(err, user) {
        if (err) console.log(err)
        let sendArray = []
        for (let deck of user.decks) {
            sendArray.push({
                deckId: deck.deckId,
                deckName: deck.deckName,
                deckDescription: deck.deckDescription,
            })
        }
        res.send(sendArray)
    })
}

exports.deck = function(req, res, next) {
    let deckId = req.body.deckId

    userDetailedModel.findOne({id: req.user._id}, function(userErr, returnedUser) {
        if (userErr) {
            console.log(userErr)
            req.status(400).send('There was an error')
        } else if (!returnedUser) {
            req.status(400).send('No user found')
        } else {
            deckModel.findOne({_id: deckId}, function(deckErr, returnedDeck) {
                if (deckErr) {
                    console.log(deckErr)
                    req.status(400).send('There was an error')
                } else if (!returnedDeck) {
                    req.status(400).send('No deck found')
                } else {
                    checkAccess(returnedUser, returnedDeck, function(result, user, deck) {
                        if (result) {
                            res.send(deck)
                        } else {
                            res.status(403).send('User does not have access to deck')
                        }
                    })
                }
            })
        }
    })
}

exports.allDecks = function(req, res, next) {
    let userId = String(req.user._id)

    userDetailedModel.findOne({id: userId}, function(userErr, returnedUser) {
        if (userErr) {
            console.log(userErr)
            res.status(400).send('There was an error')
        } else if (!returnedUser) {
            res.status(400).send('No user found')
        } else {
            var sendArray = []
            for (let deck of returnedUser.decks) {
                var quizzed = false
                if (deck.quizAttempts.length) {
                    quizzed = true
                }
                var srsed = false
                if (deck.srs.length) {
                    srsed = true
                }
                sendArray.push({
                    deckId: deck.deckId,
                    deckName: deck.deckName,
                    deckDescription: deck.deckDescription,
                    isOwner: deck.isOwner,
                    learned: deck.learned,
                    quizzed: quizzed,
                    srsed: srsed,
                })
            }
            res.send(sendArray)
        }
    })
}

exports.srs = function(req, res, next) {
    let deckId = req.body.deckId

    let sendWithoutSRS = function(deck) {
        let sendArray = []
        for (let i in deck.characters) {
            sendArray.push(deck.characters[i])
            if (parseInt(i) == 14) break //Max number of cards for review is 15
        }
        res.send(sendArray)
    }

    let sendWithSRS = function(deckInUser, deck) {
        let sendArray = []
        let srsIndexes = {}
        let deckIndexes = {}

        for (let i in deckInUser.srs) { //Indexes srs array
            srsIndexes[deckInUser.srs[i].charId] = i
        }
        for (let i in deck.characters) { //Indexes the deck, adds dummy items to srs array to aid sorting
            deckIndexes[deck.characters[i].id] = i
            if (!srsIndexes[deck.characters[i].id]) {
                deckInUser.srs.push({
                    charId: deck.characters[i].id,
                    nextDue: Date.now()
                })
            }
        }

        deckInUser.srs.sort(compare)

        for (let i in deckInUser.srs) {
            if (deckIndexes[deckInUser.srs[i].charId]) {
                sendArray.push(deck[deckIndexes[deckInUser.srs[i].charId]])
                if (parseInt(i) == 14) break //Max number of cards for review is 15
            }
        }

        res.send(sendArray)
    }

    userDetailedModel.findOne({id: req.user._id}, function(userErr, returnedUser) {
        if (userErr) {
            console.log(userErr)
            req.status(400).send('There was an error')
        } else if (!returnedUser) {
            req.status(400).send('No user found')
        } else {
            deckModel.findOne({_id: deckId}, function(deckErr, returnedDeck) {
                if (deckErr) {
                    console.log(deckErr)
                    req.status(400).send('There was an error')
                } else if (!returnedDeck) {
                    req.status(400).send('No deck found')
                } else {
                    checkAccess(returnedUser, returnedDeck, function(result, user, deck) {
                        if (result) {
                            updateLatestAccessDate(user, deck, function(result) {
                                if (result) {
                                    let deckInUser
                                    if (user.decks.length == 0) {
                                        sendWithoutSRS(deck)
                                    } else if (deckInUser = user.decks.filter( e => e.deckId == deckId)[0]) {
                                        sendWithSRS(deckInUser, deck)
                                    } else {
                                        sendWithoutSRS(deck)
                                    }
                                } else {
                                    res.status(400).send('There was an error')
                                }
                            })
                        } else {
                            res.status(403).send('User does not have access to deck')
                        }
                    })
                }
            })
        }
    })
}

exports.getDecksWithDueDates = function(req, res, next) {
    userDetailedModel.findOne({id: req.user._id}, function(err, returnedUser) {
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (returnedUser.decks.length == 0) {
            res.status(400).send('User is not using any decks with srs')
        } else {
            let sendArray = []
            for (let i of returnedUser.decks) {
                if (i.srs.length == 0) {
                    continue
                } else {
                    i.srs.sort(compare)
                    sendArray.push({
                        deckId: i.deckId,
                        deckName: i.deckName,
                        deckDescription: i.deckDescription,
                        nextDue: i.srs[0].nextDue,
                    })
                }
            }
            res.send(sendArray)
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
    let results = req.body.results //array of {id, quality}

    let writeSRS = function(resultsForSRS, deck, deckInUser) {
        let srsInUser = {}
        let validDeckIds = {}

        for (let i in deckInUser.srs) {  //Indexes the srs in user
            srsInUser[deckInUser.srs[i].charId] = i
        }
        for (let i of deck.characters) { //Indexes the deck
            validDeckIds[i.id] = true
        }

        for (let i of resultsForSRS) {
            let index = srsInUser[i.charId]  //Index of character in the userSRS array
            let srsObject = deckInUser.srs[index]
            let millisecondsInDay = 1000 * 60 *60 * 24

            if (!index) {let easiness = 2.5 + ( 0.1 - ( 5 - i.quality ) * ( 0.08 + ( 5 - i.quality) * 0.02 ) )
                let repetitions = (i.quality == 5) ? 1 : 0
                let interval = 1
                let nextDue = Date.now() + millisecondsInDay * interval

                deckInUser.srs.push({
                    charId: i.charId,
                    nextDue: nextDue,
                    interval: interval,
                    repetitions: repetitions,
                    easiness: easiness
                })
            } else {
                let easiness = srsObject.easiness + ( 0.1 - ( 5 - i.quality ) * ( 0.08 + ( 5 - i.quality) * 0.02 ) )
                let repetitions = (i.quality == 5) ? srsObject.repetitions + 1 : 0
                let interval = 1
                if (repetitions == 2) {
                    interval = 6
                } else if (repetitions > 2) {
                    interval = Math.ceil(srsObject.interval * easiness)
                    console.log(srsObject)
                }
                let nextDue = Date.now() + millisecondsInDay * interval

                srsObject.nextDue = nextDue
                srsObject.interval = interval
                srsObject.repetitions = repetitions
                srsObject.easiness = easiness
            }
        }
    }

    let initNewDeckInUser = function(user, deckId, deckName, deckDescription) {
        user.decks.push({deckId: deckId, deckName: deckName, deckDescription: deckDescription})
    }

    let saveUser = function(userToSave, deckInfo) {
        updateLatestAccessDate(userToSave, deckInfo, function(result) { //Saves user as well
            if (result) {
                res.sendStatus(200)
            } else {
                res.status(400).send('There was an error')
            }
        })
    }

    userDetailedModel.findOne({id: String(req.user._id)}, function(userErr, returnedUser) {
        if (userErr) {
            console.log(userErr)
            res.status(400).send('There was an error while finding user')
        } else if (!returnedUser) {
            res.status(400).send('No user found')
        } else if (!results) {
            res.status(400).send('No results sent')
        } else if (!deckId) {
            res.status(400).send('No deck sent')
        } else {
            deckModel.findOne({_id: deckId}, function(deckErr, returnedDeck) {
                if (deckErr) {
                    console.log(deckErr)
                    res.status(400).send('There was an error while finding decks')
                } else if (!returnedDeck) {
                    res.status(400).send('No deck found')
                } else {
                    checkAccess(returnedUser, returnedDeck, function(result, user, deck) {
                        if (result) {
                            let deckInUser
                            if (user.decks.length == 0) {
                                initNewDeckInUser(user, deckId, deck.title, deck.description)
                                writeSRS(results, deck, user.decks[0])
                                saveUser(user, deck)
                            } else if (deckInUser = user.decks.filter( e => e.deckId == deckId)[0]) {
                                writeSRS(results, deck, deckInUser)
                                saveUser(user, deck)
                            } else {
                                initNewDeckInUser(user, deckId, deck.title, deck.description)
                                writeSRS(results, deck, user.decks[user.decks.length - 1])
                                saveUser(user, deck)
                            }
                        } else {
                            res.status(403).send('User does not have access to deck')
                        }                        
                    })
                }
            })
        }
    })    
}

exports.quizzed = function(req, res, next) {
    let deckId = req.body.deck
    let results = req.body.results

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

    let initNewDeckInUser = function(user, deckId, deckName, deckDescription) {
            user.decks.push({deckId: deckId, deckName: deckName, deckDescription: deckDescription})
    }

    let saveUser = function(userToSave, deckInfo) {
        updateLatestAccessDate(userToSave, deckInfo, function(result) { //Saves user as well
            if (result) {
                res.sendStatus(200)
            } else {
                res.status(400).send('There was an error')
            }
        })
    }

    userDetailedModel.findOne({id: String(req.user._id)}, function(userErr, returnedUser) { //user_err might always evaluate to true tho /s
        //Doesn't use "_id" because "_id" autogenerated, linked to main user db with "id"
        if (userErr) console.log(userErr)

        if (!returnedUser) {
            res.status(400).send('No user found')
        } else if (!results) {
            res.status(400).send("No results sent")
        } else {
            deckModel.findOne({_id: deckId}, function(deck_err, returnedDeck) {
                if (deck_err) console.log(deck_err)

                if (!returnedDeck) {
                    res.status(400).send("Could not find deck")
                } else {
                    checkAccess(returnedUser, returnedDeck, function(result, user, deck) {
                        if (result) {
                            let deckInUser
                            if (user.decks.length == 0) {
                                initNewDeckInUser(user, deckId, deck.title, deck.description)
                                writeResultsToUser(results, user.decks[0], 0)
                                saveUser(user, returnedDeck)
                            } else if (deckInUser = user.decks.filter( e => e.deckId == deckId)[0]) {
                                writeResultsToUser(results, deckInUser, deckInUser.totalQuizAttempts)
                                saveUser(user, returnedDeck)
                            } else {
                                initNewDeckInUser(user, deckId, deck.title, deck.description)
                                writeResultsToUser(results, user.decks[user.decks.length - 1], 0)
                                saveUser(user, returnedDeck)
                            }
                        } else {
                            res.status(403).send('User does not have access to deck')
                        }
                    })                   
                }
            })
        }
    })
}

exports.learned = function(req, res, next) {
    let deckId = req.body.deckId
    let userId = String(req.user._id)

    userDetailedModel.findOne({id: userId}, function(userErr, returnedUser) {
        if (userErr) console.log(userErr)

        if (!returnedUser) {
            res.status(400).send('No user found')
        } else {
            deckModel.findOne({_id: deckId}, function(deck_err, returnedDeck) {
                if (deck_err) console.log(deck_err)

                if (!returnedDeck) {
                    res.status(400).send("Could not find deck")
                } else {
                    checkAccess(returnedUser, returnedDeck, function(result, user, deck) {
                        if (result) {
                            checkIfDeckEntryExists(user, deck._id, function(i) {
                                if (i !== false) {
                                    user.decks[i].learned = true
                                } else {
                                    user.decks.push({
                                        deckId: deck._id,
                                        deckName: deck.title,
                                        deckDescription: deck.description,
                                        learned: true,
                                    })
                                }
                            })

                            updateLatestAccessDate(user, deck, function(result) {
                                if (result) {
                                    res.sendStatus(200)
                                } else{
                                    res.status(400).send('There was an error')
                                }
                            })
                        } else {
                            res.status(403).send('User does not have access to deck')
                        }
                    })                   
                }
            })
        }
    })
}
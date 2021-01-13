var classModel = require('../models/classModel')
var userDetailedModel = require('../models/userDetailedModel')
var deckModel = require('../models/deckModel')
var { validationResult } = require('express-validator')

let saveClassAndDeck = function(classToSave, deckToSave, res) {
    classToSave.save(function(classErr) {
        if (classErr) {
            res.status(400).send(classErr)
        } else {
            deckToSave.save(function(deckErr) {
                if (deckErr) {
                    res.status(400).send(deckErr)
                } else {
                    res.sendStatus(200)
                }
            })
        }
    })
}

exports.createClass = function(req, res, next) {
    if (req.user.isTeacher == false) {
        res.sendStatus(403)
    } else {
        let teacherId = req.user._id
        let teacherName = req.user.firstName + " " + req.user.lastName
        let className = req.body.className || "No name provided"
        let description = req.body.description || "No description provided"
        let private = req.body.private || true

        let createClass = function() {
            let classCode = Math.floor(100000 + Math.random() * 900000)

            classModel.findOne({classCode: classCode}, function(err, result) {
                if (result) {
                    createClass()
                } else {
                    var Class = new classModel({
                        teacherId: teacherId,
                        teacherName: teacherName,
                        name: className,
                        description: description,
                        private: private,
                        classCode: classCode,
                    })
                    Class.save(function(err) {
                        if (err) {
                            console.log(err)
                            res.sendStatus(400)
                        } else {
                            userDetailedModel.findOne({id: teacherId}, function(err, returnedTeacher) {
                                if (err) {
                                    console.log(err)
                                    res.status(400).send('There was an error')
                                } else {
                                    returnedTeacher.classesTeaching.push(classCode)
                                    returnedTeacher.save()
                                    res.sendStatus(200)
                                }
                            })
                        }
                    })
                }
            })
        }

        createClass()
    }
}

exports.removeClass = function(req, res, next) {
    let teacher = String(req.user._id)
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!Class) {
            res.status(400).send('No class found')
        } else if (Class.teacherId != teacher) {
            res.sendStatus(401)
        } else if (Class.teacherId == teacher) {
            classModel.deleteOne({_id: Class._id}, function(err) {
                if (err) console.log(err)
                res.sendStatus(200)
            })
        } else {
            res.sendStatus(400)
        }
    })
}

exports.join = function(req, res, next) {
    let studentId = String(req.user._id)
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.sendStatus(400)
        } else if (!Class) {
            res.status(400).send('Class does not exist')
        }  else if (Class.teacherId == studentId) {
            res.send('User cannot join a class they teach')
        } else if (Class.students.length == 0) {
            Class.students.push({
                firstName: req.user.firstName,
                lastName: req.user.lastName,
                id: studentId
            })
            Class.save()
            addClassToStudent()
            res.sendStatus(200)
        } else {
            for (let i in Class.students) {
                if (String(Class.students[i].id) == studentId) {
                    res.status(400).send("Student already in class")
                    break
                } else if (parseInt(i) + 1 == Class.students.length) {
                    Class.students.push({
                        firstName: req.user.firstName,
                        lastName: req.user.lastName,
                        id: studentId
                    })
                    Class.save()
                    addClassToStudent()
                    res.sendStatus(200)
                    break
                }
            }
        }
    })

    let addClassToStudent = function() {
        userDetailedModel.findOne({id: studentId}, function(err, result) { //Id here is different from _id
            if (err) {
                console.log(err)
            } else {
                result.classes.push(classCode)
                result.save()
            }
        })
    }
}

exports.leave = function(req, res, next) {
    let student = String(req.user._id)
    let classCode = req.body.classCode

    let removeClassFromStudent = function() {
        userDetailedModel.findOne({id: student}, function(err, result) {
            if (err) {
                console.log(err)
            } else {
                for (let j in result.classes) {
                    if (result.classes[j] == classCode) {
                        result.classes.splice(j, 1)
                        result.save()
                    }
                }
            }
        })
    }

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.status(400).send("There was an error")
        } else if (student == Class.teacherId) {
            res.status(400).send('Teacher cannot leave class they teach')
        } else if (Class.students.length == 0) {
            res.status(400).send('Student not in class')
        } else {
            for (let i in Class.students) {
                if (Class.students[i].id == student) {
                    Class.students.splice(i, 1)
                    Class.save()
                    removeClassFromStudent()
                    res.sendStatus(200)
                    break
                } else if (parseInt(i) + 1 == Class.students.length) {
                    res.status(400).send('Student not in class')
                }
            }
        }
    })   
}

exports.getJoinedClasses = function(req, res, next) {
    let userId = String(req.user._id)

    userDetailedModel.findOne({id: userId}, function(userErr, user) {
        if (userErr) {console.log(userErr)}

        if (!user) {
            res.status(400).send('No user found')
        } else {
            let classCodeArray = []

            for (let Class of user.classes) {
                classCodeArray.push({
                    classCode: Class
                })
            }

            if (classCodeArray.length) {
              classModel.find({ $or: classCodeArray}, function(classErr, classes) {
                  if (classErr) {console.log(classErr)}
      
                  if (!classes) {
                      res.status(400).send('User has not joined any classes')
                  } else {
                      let classArray = []

                      for (let Class of classes) { // bypass js keyword
                          classArray.push({
                              name: Class.name,
                              description: Class.description,
                              teacherName: Class.teacherName,
                              classCode: Class.classCode,
                          })
                      }

                      res.status(200).send(classArray)
                  }
              })
            } else {
              res.status(400).send('User has not joined any classes')
            }
        }        
    })
}

exports.getTeachingClasses = function(req, res, next) {
    let teacherId = String(req.user._id)

    userDetailedModel.findOne({id: teacherId}, function(userErr, teacher) {
        if (userErr) {console.log(userErr)}

        if (!teacher) {
            res.status(400).send('No user found')
        } else if (teacher.isTeacher == false) {
            res.status(403).send('User is not a teacher')
        } else {
            let classCodeArray = []

            for (let Class of teacher.classesTeaching) {
                classCodeArray.push({
                    classCode: Class
                })
            }

            if (classCodeArray.length) {
              classModel.find({ $or: classCodeArray}, function(classErr, classes) {
                  if (classErr) {console.log(classErr)}
      
                  if (!classes) {
                      res.send('User is not teaching any classes')
                  } else {
                      let classArray = []

                      for (let Class of classes) {
                          classArray.push({
                              name: Class.name,
                              description: Class.description,
                              teacherName: Class.teacherName,
                              classCode: Class.classCode,
                          })
                      }

                      res.status(200).send(classArray)
                  }
              })
            } else {
              res.status(400).send('User is not teaching any classes')
            }
        }        
    })
}

exports.Class = function(req, res, next) {
    let classCode = req.body.classCode
    let userId = String(req.user._id)

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (!Class) {
            res.status(400).send('No class found')
        } else {
            
            if (Class.teacherId == userId) {
                res.send(Class)
            } else {
                if (!Class.students) {
                    res.status(403).send('User does not teach this class or is not in this class')
                } else {
                    for (let i in Class.students) {
                        if (Class.students[i] == userId) {
                            res.send(Class)
                            break
                        } else if (parseInt(i) + 1 == Class.students.length) {
                            res.status(403).send('User does not teach this class or is not in this class')
                        }
                    }
                } 
            }
        }
    })
}

exports.assign = function(req, res, next) {
    let errors = validationResult(req)

    if (!errors.isEmpty()) {
        res.status(400).json({ errors: errors.array() });
    } else {
        let teacher = String(req.user._id)
        let classCode = req.body.classCode //Required
        let deckId = req.body.deckId //Required
        let mode = req.body.mode //Required. Allowed: ("learn", "srs", "quiz")
        let handwriting = req.body.handwriting //Defaults to false
        let front = req.body.front //Required. Allowed: ("character", "pinyin", "definition", "handwriting")
        let scramble = req.body.scramble //Defaults to false
        let repetitions = req.body.repetitions //Defaults to 1
        let dueDate = req.body.dueDate //Required. epoch time in milliseconds

        let checkIfDeckIsAssigned = function(Class, deckId, deck) {
            if (!Class.assignedDecks.length) {
                assignDeck(Class, deckId, deck)
            } else {
                for (let i in Class.assignedDecks) {
                    if (Class.assignedDecks[i] == deckId) {
                        res.status(400).send("Deck already assigned")
                        break
                    } else if (parseInt(i) + 1 == Class.assignedDecks.length) {
                        assignDeck(Class, deckId, deck)
                    }
                }
            }
        }

        let assignDeck = function (Class, deckId, deck) {
            if (mode != "quiz" && front == "handwriting") {
                res.status(400).send('The front card being set to handwriting is only compatible with quiz mode')
            } else if (isNaN(parseInt(repetitions))) {
                res.status(400).send('If a repetitions value is sent, it must be an integer')
            } else if (mode != "learn" && scramble) {
                res.status(400).send('Only learn mode supports scramble')
            } else {
                if (!repetitions) {
                    repetitions = 1
                }
                Class.assignedDecks.push({
                    deckId: deckId,
                    deckName: deck.title,
                    deckDescription: deck.description,
                    mode: mode,
                    handwriting: handwriting,
                    front: front,
                    scramble: scramble,
                    repetitions: repetitions,
                    assignedDate: Date.now(),
                    dueDate: dueDate,
                })
                deck.access.classes[Class.classCode] = true
                saveClassAndDeck(Class, deck, res)
            }
        }
        
        classModel.findOne({classCode: classCode}, function(classErr, Class) {
            if (classErr) {
                console.log(classErr)
                res.status(400).send("There was an error")
            } else if (!Class) {
                res.status(400).send("The class does not exist")
            } else if (Class.teacherId != teacher) {
                res.sendStatus(403)
            } else {
                deckModel.findOne({_id: deckId}, function(deckErr, returnedDeck) {
                    if (deckErr) {
                        console.log(deckErr)
                        res.status(400).send('There was an error')
                    } else if (!returnedDeck) {
                        res.status(400).send('Deck does not exist')
                    } else if (returnedDeck.access.isPublic == true) {
                        checkIfDeckIsAssigned(Class, deckId, returnedDeck)
                    } else if (returnedDeck.creator == teacher) {
                        checkIfDeckIsAssigned(Class, deckId, returnedDeck)
                    } else {
                        res.status(403).send('User does not have permission to access deck')
                    }
                })
            }
        })
    }
}

exports.unassign = function(req, res, next) {
    let teacher = req.user._id
    let classCode = req.body.classCode
    let deckId = req.body.deckId

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("There was an error")
        } else if (!Class) {
            res.status(400).send('No class found')
        } else if (Class.teacherId != teacher) {
            res.sendStatus(403)
        } else {
            if (Class.assignedDecks.length == 0) {
                res.status(400).send("No decks to remove")
            } else {
                deckModel.findOne({_id: deckId}, function(deckErr, returnedDeck) {
                    if (deckErr) {
                        console.log(deckErr)
                        res.status(400).send('There was an error')
                    } else {
                        for (let i in Class.assignedDecks) {
                            if (Class.assignedDecks[i].deckId == deckId) {
                                Class.assignedDecks.splice(i, 1)
                                delete returnedDeck.access[classCode]
                                saveClassAndDeck(Class, returnedDeck, res)
                                break
                            } else if (parseInt(i) + 1 == Class.assignedDecks.length) {
                                res.status(400).send("Deck not assigned")
                            }
                        }
                    }
                })
            }
        }
    })
}


exports.getAssignedDecksAsStudent = function(req, res, next) {
    let user = String(req.user._id)
    let classCode = req.body.classCode

    let sendDecks = function(assignedDecks) {
        var deckArray = []

        for (let deck of assignedDecks) {
            let status

            if (deck.mode == "learn" || deck.mode == "quiz") {
                if (!deck.results[user]) {
                    status = "Not started"
                } else if (deck.results[user].done) {
                    status = "Finished"
                } else {
                    status = "Not started"
                }
            } else if (deck.mode == "srs") {
                if (!deck.results[user]) {
                    status = `0/${deck.repetitions}. Not started`
                } else if (!deck.results[user].repetitions) {
                    status = `0/${deck.repetitions}. Not started`
                } else if (deck.results[user].repetitions < deck.repetitions) {
                    status = `${deck.results[user].repetitions}/${deck.repetitions}. Not finished`
                } else {
                    status = `${deck.results[user].repetitions}/${deck.repetitions}. Finished`
                }
            }

            deckArray.push({
                deckId: deck.deckId,
                deckName: deck.deckName,
                deckDescription: deck.deckDescription,
                mode: deck.mode,
                handwriting: deck.handwriting,
                front: deck.front,
                scramble: deck.scramble,
                repetitions: deck.repetitions,
                assignedDate: deck.assignedDate,
                dueDate: deck.dueDate,
                status: status
            })
        }

        res.send(deckArray)
    }

    classModel.findOne({classCode: classCode}, function(err, Class) {        
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!Class) {
            res.status(400).send('No class was found')            
        } else if (Class.assignedDecks.length == 0) {
            res.status(200).send('No decks are assigned')
        } else if (Class.teacherId == user) {
            res.status(400).send('Teachers cannot request decks as a student')
        } else {
            for (let i in Class.students) {
                if (user == Class.students[i]) {
                    sendDecks(Class.assignedDecks)
                    break
                } else if (parseInt(i) + 1 == Class.students.length) {
                    res.send(403).send('Only students of the class can access the classes decks')
                }
            }
        }
    }) 
}

exports.getAssignedDecksAsTeacher = function(req, res, next) {
    let teacher = String(req.user._id)
    let classCode = req.body.classCode

    let sendDecks = function(Class) {
        var deckArray = []

        for (let deck of Class.assignedDecks) {
            let status
            let numberOfStudentsFinished = 0

            for (let student of deck.results) {
                if (student.done) {
                    numberOfStudentsFinished++
                }
            }

            status = `${numberOfStudentsFinished}/${Class.students.length}`

            deckArray.push({
                deckId: deck.deckId,
                deckName: deck.deckName,
                deckDescription: deck.deckDescription,
                mode: deck.mode,
                handwriting: deck.handwriting,
                front: deck.front,
                scramble: deck.scramble,
                repetitions: deck.repetitions,
                assignedDate: deck.assignedDate,
                dueDate: deck.dueDate,
                status: status
            })
        }

        res.send(deckArray)
    }

    classModel.findOne({classCode: classCode}, function(err, Class) {        
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!Class) {
            res.status(400).send('No class was found')            
        } else if (Class.assignedDecks.length == 0) {
            res.status(200).send('No decks are assigned')
        } else if (Class.teacherId == teacher) {
            sendDecks(Class)
        } else {
            res.sendSatus(403)
        }
    }) 
}

exports.submitFinishedDeck = function(req, res, next) {
    let user = String(req.user._id)
    let deckId = req.body.deckId
    let classCode = req.body.classCode
    let mode = req.body.mode //Required. Allowed: ("learn", "srs", "quiz")
    let quizResults = req.body.results //Only needed if mode is quiz

    let writeSubmitedDeck = function(deckInClass, deckInDB) {
        if (mode == 'learn') {
            deckInClass.result[user].done = true
        } else if (mode == 'quiz') {
            deckInClass.result[user].done = true
            deckInClass.result[user].quizStats = processQuizResults(quizResults, deckInDB)
        } else if (mode == 'srs') {
            if (!deckInClass.results[user].repetitions) {
                deckInClass.results[user].repetitions = 1
            } else {
                deckInClass.results[user].repetitions++
            }

            if (deckInClass.results[user].repetitions == deckInClass.repetitions) {
                deckInClass.result[user].done = true
            }
            
            return
        }
    }

    let processQuizResults = function(results, deckInDB) {
        let writeArray = []
        let charactersInDeck = {}
        let numCorrect = 0
        let overriden = 0

        for (let char of deckInDB) {
            charactersInDeck[char.id] = char.char
        }
        
        for (let i of results) {
            writeArray.push({char: charactersInDeck[i.id],charId: i.id, correct: i.correct, overriden: i.overriden})
            if (i.correct == true) {
                numCorrect++
            }
            if (i.overriden == true) {
                overriden++
            }
        }

        return ({
            summary: {
                correct: numCorrect,
                total: results.length,
                overriden: overriden,
            },
            stats: writeArray
        })
    }

    classModel.findOne({classCode: classCode}, function(err, Class) {        
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!Class) {
            res.status(400).send('No class was found')            
        } else if (Class.assignedDecks.length == 0) {
            res.status(200).send('No decks are assigned')
        } else if (Class.teacherId == user) {
            res.status(400).send('Teachers cannot submit decks')
        } else {
            for (let i in Class.students) {
                if (user == Class.students[i]) {
                    for (let j in Class.assignedDecks) {
                        if (Class.assignedDecks[j].deckId == deckId) {
                            deckModel.findOne({_id: deckId}, function(deck_err, returnedDeck) {
                                if (deck_err) console.log(deck_err)

                                writeSubmitedDeck(Class.assignedDecks[j], returnedDeck)
                                Class.save()
                                res.sendStatus(200)
                            })
                            break
                        } else if (parseInt(j) + 1 == Class.assignedDecks.length) {
                            res.status(400).send('This deck was not assigned')
                        }
                    }
                    break
                } else if (parseInt(i) + 1 == Class.students.length) {
                    res.send(403).send('Only students of the class can submit to the class')
                }
            }
        }
    }) 
}

exports.getDeckResults = function(req, res, next) {
    let teacher = String(req.user._id)
    let deckId = req.body.deckId
    let classCode = req.body.classCode

    let sendDeckResults = function(Class, assignedDeck) {
        let resultsArray = []

        if (assignedDeck.mode == 'learn') {

            let sendToResults = function(student, finished) {
                resultsArray.push({
                    firstName: student.firstName,
                    lastName: student.lastName,
                    id: student.id,
                    finished: finished,
                })
            }

            for (let student of Class.students) {
                if (!assignedDeck.results[student.id]) {
                    sendToResults(student, false)
                } else if (!assignedDeck.results[student.id].done) {
                    sendToResults(student, false)
                } else {
                    sendToResults(student, true)
                }
            }
        } else if (assignedDeck.mode == 'quiz') {

            let sendToResults = function(student, finished, quizStats) {
                resultsArray.push({
                    firstName: student.firstName,
                    lastName: student.lastName,
                    id: student.id,
                    finished: finished,
                    quizStats: quizStats,
                })
            }

            for (let student of Class.students) {
                if (!assignedDeck.results[student.id]) {
                    sendToResults(student, false, undefined)
                } else if (!assignedDeck.results[student.id].done) {
                    sendToResults(student, false, undefined)
                } else {
                    sendToResults(student, true, assignedDeck.results[student.id].quizStats)
                }
            }
        } else if (assignedDeck.mode == 'srs') {
            let sendToResults = function(student, finished, repetitionsFinished) {
                resultsArray.push({
                    firstName: student.firstName,
                    lastName: student.lastName,
                    id: student.id,
                    finished: finished,
                    repetitionsFinished: repetitionsFinished,
                    repetitionsTotal: assignedDeck.repetitions
                })
            }

            for (let student of Class.students) {
                if (!assignedDeck.results[student.id]) {
                    sendToResults(student, false, 0)
                } else if (!assignedDeck.results[student.id].done) {
                    sendToResults(student, false, assignedDeck.results[student.id].repetitions || 0)
                } else {
                    sendToResults(student, true, assignedDeck.results[student.id].repetitions)
                }
            }
        }

        res.send(resultsArray)
    }    

    classModel.findOne({classCode: classCode}, function(err, Class) {        
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!Class) {
            res.status(400).send('No class was found')            
        } else if (Class.assignedDecks.length == 0) {
            res.status(200).send('No decks are assigned')
        } else if (Class.teacherId == teacher) {
            for (let i in Class.assignedDecks) {
                if (deckId == Class.assignedDecks[i].deckId) {
                    sendDeckResults(Class, Class.assignedDecks[i])
                    break
                } else if (parseInt(i) + 1 == Class.assignedDecks.length) {
                    res.send(400).send('Deck not assigned')
                }
            }
        } else {
            res.sendSatus(403)
        }
    }) 
}

//get assigned decks as student (get class as student)  DONE
    //go through all assigned decks
    //for each deck, send everything except for results
    //if learn, send if it was done or not
    //if quiz, send score if it was done
    //if srs, send how many more repetitions needed, or done
//get assigned decks as teacher (get class as teacher) DONE
    //send everything, except for results
//submit finished deck (only for students) DONE
    //get results of whatever they did, put those results into results array with their name, under their id
//see results of deck as teacher DONE
    //send everything, including results

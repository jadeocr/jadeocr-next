var classModel = require('../models/classModel')
var userDetailedModel = require('../models/userDetailedModel')
var deckModel = require('../models/deckModel')

exports.createClass = function(req, res, next) {
    if (req.user.isTeacher == false) {
        res.sendStatus(403)
    } else {
        let teacher = req.user._id
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
                        teacher: teacher,
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
                            userDetailedModel.findOne({id: teacher}, function(err, returnedTeacher) {
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
    let teacher = req.user._id
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send('There was an error')
        } else if (!Class) {
            res.send('No class found')
        } else if (Class.teacher != teacher) {
            res.sendStatus(401)
        } else if (Class.teacher == teacher) {
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
    let student = String(req.user._id)
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.sendStatus(400)
        } else if (!Class) {
            res.status(400).send('Class does not exist')
        }  else if (Class.teacher == student) {
            res.send('User cannot join a class they teach')
        } else if (Class.students.length == 0) {
            Class.students.push(student)
            Class.save()
            addClassToStudent()
            res.sendStatus(200)
        } else {
            for (let i in Class.students) {
                if (String(Class.students[i]) == student) {
                    res.send("Student already in class")
                    break
                } else if (parseInt(i) + 1 == Class.students.length) {
                    Class.students.push(student)
                    Class.save()
                    addClassToStudent()
                    res.sendStatus(200)
                    break
                }
            }
        }
    })

    let addClassToStudent = function() {
        userDetailedModel.findOne({id: student}, function(err, result) { //Id here is different from _id
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

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("There was an error")
        } else if (student == Class.teacher) {
            res.send('Teacher cannot leave class they teach')
        } else if (Class.students.length == 0) {
            res.send('Student not in class')
        } else {
            for (let i in Class.students) {
                if (Class.students[i] == student) {
                    Class.students.splice(i, 1)
                    Class.save()
                    removeClassFromStudent()
                    res.sendStatus(200)
                } else if (parseInt(i) + 1 == Class.students.length) {
                    res.send('Student not in class')
                }
            }
        }
    })
    
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
}

exports.assign = function(req, res, next) {
    let teacher = String(req.user._id)
    let classCode = req.body.classCode
    let deckId = req.body.deck

    let saveClassAndDeck = function(classToSave, deckToSave) {
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

    let assignDeck = function(Class, deckId, deck) {
        if (Class.assignedDecks.length == 0) {
            Class.assignedDecks.push(deckId)
            deck.access.classes[Class.classCode] = true
            saveClassAndDeck(Class, deck)
        } else {
            for (let i in Class.assignedDecks) {
                if (Class.assignedDecks[i] == deckId) {
                    res.status(400).send("Deck already assigned")
                } else if (parseInt(i) + 1 == Class.assignedDecks.length) {
                    Class.assignedDecks.push(deckId)
                    deck.access.classes[Class.classCode] = true
                    saveClassAndDeck(Class, deck)
                }
            }
        }
    }
    
    classModel.findOne({classCode: classCode}, function(classErr, Class) {
        if (classErr) {
            console.log(classErr)
            res.status(400).send("There was an error")
        } else if (!Class) {
            res.status(400).send("The class does not exist")
        } else if (Class.teacher != teacher) {
            res.sendStatus(403)
        } else {
            deckModel.findOne({_id: deckId}, function(deckErr, returnedDeck) {
                if (deckErr) {
                    console.log(deckErr)
                    res.status(400).send('There was an error')
                } else if (returnedDeck.access.isPublic == true) {
                    assignDeck(Class, deckId, returnedDeck)
                } else if (returnedDeck.creator == teacher) {
                    assignDeck(Class, deckId, returnedDeck)
                } else {
                    res.status(403).send('User does not have permission to access deck')
                }
            })
        }
    })
}

exports.unassign = function(req, res, next) {
    let teacher = req.user._id
    let classCode = req.body.classCode
    let deckId = req.body.deck

    let saveClassAndDeck = function(classToSave, deckToSave) {
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

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("The class does not exist")
        } else if (Class.teacher != teacher) {
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
                            if (Class.assignedDecks[i] == deckId) {
                                Class.assignedDecks.splice(i, 1)
                                delete returnedDeck.access[classCode]
                                saveClassAndDeck(Class, returnedDeck)
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

exports.getAssignedDecks = function(req, res, next) {
    let user = String(req.user._id)
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        let sendDecks = function() {
            var deckArray = []

            for (let j in Class.assignedDecks) {
                deckArray.push({_id: Class.assignedDecks[j]})
                if (parseInt(j) + 1 == Class.assignedDecks.length) {
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
        
        if (err) {
            console.log(err)
            res.status(400).send('There was an error')
        } else if (!Class) {
            res.status(400).send('No class was found')            
        } else if (Class.assignedDecks.length == 0) {
            res.status(200).send('No decks are assigned')
        } else if (Class.teacher == user) {
            sendDecks()
        } else {
            for (let i in Class.students) {
                if (user == Class.students[i]) {
                    sendDecks()
                    break
                } else if (parseInt(i) + 1 == Class.students.length) {
                    res.send(403)
                }
            }
        }
    })
}
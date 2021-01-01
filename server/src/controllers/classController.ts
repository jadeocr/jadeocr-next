var classModel = require('../models/classModel')
var userDetailedModel = require('../models/userDetailedModel')
var deckModel = require('../models/deckModel')

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
            res.send('There was an error')
        } else if (!Class) {
            res.send('No class found')
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
    let student = String(req.user._id)
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.sendStatus(400)
        } else if (!Class) {
            res.status(400).send('Class does not exist')
        }  else if (Class.teacherId == student) {
            res.send('User cannot join a class they teach')
        } else if (Class.students.length == 0) {
            Class.students.push(student)
            Class.save()
            addClassToStudent()
            res.sendStatus(200)
        } else {
            for (let i in Class.students) {
                if (String(Class.students[i]) == student) {
                    res.status(400).send("Student already in class")
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
        } else if (student == Class.teacherId) {
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
    let teacher = String(req.user._id)
    let classCode = req.body.classCode
    let deckId = req.body.deck

    let assignDeck = function(Class, deckId, deck) {
        if (!Class.assignedDecks.length) {
            Class.assignedDecks.push(deckId)
            deck.access.classes[Class.classCode] = true
            saveClassAndDeck(Class, deck, res)
        } else {
            for (let i in Class.assignedDecks) {
                if (Class.assignedDecks[i] == deckId) {
                    res.status(400).send("Deck already assigned")
                } else if (parseInt(i) + 1 == Class.assignedDecks.length) {
                    Class.assignedDecks.push(deckId)
                    deck.access.classes[Class.classCode] = true
                    saveClassAndDeck(Class, deck, res)
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

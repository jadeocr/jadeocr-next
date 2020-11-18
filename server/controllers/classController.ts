//TODO: Make the variables reference the right things.

var classModel = require('../models/classModel')
var userDetailedModel = require('../models/userDetailedModel')

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
                            res.sendStatus(200)
                        }
                    })
                }
            })
        }

        createClass()
    }
}

exports.join = function(req, res, next) {
    let student = String(req.user._id)
    let classCode = req.body.classCode

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("There was an error")
        } else if (!Class) {
            res.send('Class not found')
        }  else if (Class.teacher == student) {
            res.send('User cannot join a class they teach')
        } else if (Class.students.length == 0) {
            Class.students.push(student)
            Class.save()
            res.sendStatus(200)
        } else {
            for (let i in Class.students) {
                if (String(Class.students[i]) == student) {
                    res.send("Student already in class")
                    break
                } else if (i + 1 == Class.students.length) {
                    Class.students.push(student)
                    Class.save()
                    res.sendStatus(200)
                    break
                }
            }
        }
    })
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
                    res.sendStatus(200)
                } else if (i + 1 == Class.students.length) {
                    res.send('Student not in class')
                }
            }
        }
    })
}

exports.assign = function(req, res, next) {
    let teacher = String(req.user._id)
    let classCode = req.body.classCode
    let deck = req.body.deck

    classModel.findOne({classCode: classCode}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("The class does not exist")
        } else if (Class.teacher != teacher) {
            res.sendStatus(403)
        } else {
            if (Class.assignedDecks.length == 0) {
                Class.assignedDecks.push(deck)
                res.sendStatus(200)
            } else {
                for (let i in Class.assignedDecks) {
                    if (Class.assignedDecks[i] == deck) {
                        res.send("Deck already assigned")
                    } else if (i + 1 == Class.assignedDecks.length) {
                        Class.assignedDecks.push(deck)
                        res.sendStatus(200)
                    }
                }
            }
        }
    })
}

exports.unassign = function(req, res, next) {
    let teacher = req.user.user
    let className = req.body.classname
    let deck = req.body.deck

    if (req.user.isTeacher == false) {
        res.sendStatus(403)
    }

    classModel.findOne({name: className}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("The class does not exist")
        } else if (Class.teacher != teacher) {
            res.sendStatus(403)
        } else {
            if (!Class.assignedDecks) {
                res.send("No decks to remove")
            } else {
                for (let i in Class.assignedDecks) {
                    if (Class.assignedDecks[i] == deck) {
                        Class.assignedDecks.splice(i, 1)
                        res.sendStatus(200)
                    }
                }
                res.send("Deck not assigned")    // <<---- not sure if this works, not tested
            }
        }
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
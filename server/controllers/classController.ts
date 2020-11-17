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
        
        var Class = new classModel({
            teacher: teacher,
            name: className,
            description: description,
            private: private,
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
}

exports.join = function(req, res, next) {
    let student = req.body.user
    let className = req.body.classname

    classModel.findOne({name: className}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("The class does not exist")
        } else if (Class.private == false) {
            for (let i of Class.students) {
                if (i == student) {
                    res.send("Student already in class")
                } else {
                    Class.students.push(student)
                    res.sendStatus(200)
                }
            }
        } else if (Class.private == true) {
            if (!Class.invites) {
                res.send("Student does not have invite")
            } else {
                for (let i of Class.invites) {
                    if (student == Class.invites[i]) {
                        Class.students.push(student)
                        res.sendStatus(200)
                    } else {
                        res.send("Student does not have invite")
                    }
                }
            }
        } 
    })
}

exports.leave = function(req, res, next) {
    let student = req.body.user
    let className = req.body.classname

    classModel.findOne({name: className}, function(err, Class) {
        if (err) {
            console.log(err)
            res.send("The class does not exist")
        } else {
            for (let i in Class.students) {
                if (Class.students[i] == student) {
                    Class.students.splice(i, 1)
                    res.sendStatus(200)
                }
            }
        }
    })
}

exports.assign = function(req, res, next) {
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
                Class.assignedDecks.push(deck)
                res.sendStatus(200)
            } else {
                for (let i of Class.assignedDecks) {
                    if (i == deck) {
                        res.send("Deck already assigned")
                    } else {
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
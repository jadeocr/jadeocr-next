import router from '../../router/index'
import axios from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL
import { ClassI } from '../../interfaces/Class'
// import { Deck } from '../../interfaces/Deck'
import { AssignedDeck } from '../../interfaces/AssignedDeck'

export const classes = {
  namespaced: true,
  state: {
    classErrMsg: '',
    classes: Array<ClassI>(),
    classesTeaching: Array<ClassI>(),
    currClassAssignments: Array<AssignedDeck>(),
  },
  mutations: {
    // eslint-disable-next-line
    setClasses(state: any, classes: Array<ClassI>) {
      state.classes = classes
    },
    // eslint-disable-next-line
    setClassesTeaching(state: any, classesTeaching: Array<ClassI>) {
      state.classesTeaching = classesTeaching
    },
    // eslint-disable-next-line
    setClassErrMsg(state: any, msg: string) {
      state.classErrMsg = msg
    },
    // eslint-disable-next-line
    setCurrClassAssignments(state: any, decks: Array<AssignedDeck>) {
      state.currClassAssignments = decks
    },
  },
  actions: {
    getClassesTeaching({
      commit,
      rootState,
    }: {
      commit: Function
      // eslint-disable-next-line
      rootState: any
    }): void {
      if (rootState.auth.isTeacher) {
        axios({
          method: 'post',
          withCredentials: true,
          url: `${apiBaseURL}/class/getTeachingClasses`,
        })
          .then((res) => {
            commit('setClassesTeaching', res.data)
          })
          .catch((err) => {
            console.log(err.response.data)
          })
      }
    },
    getClassesJoined({ commit }: { commit: Function }): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/getJoinedClasses`,
      })
        .then((res) => {
          commit('setClasses', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
          if (err.response.data == 'User has not joined any classes') {
            commit('setClasses', Array<ClassI>())
          }
        })
    },
    joinClass({ commit }: { commit: Function }, classCode: string): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/join`,
        data: {
          classCode: classCode,
        },
      })
        .then(() => {
          commit('setClassErrMsg', '')
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          commit('setClassErrMsg', err.response.data)
          console.log(err.response.data)
        })
    },
    leaveClass({ commit }: { commit: Function }, classCode: string): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/leave`,
        data: {
          classCode: classCode,
        },
      })
        .then(() => {
          commit('setClassErrMsg', '')
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setClassErrMsg', err.response.data)
        })
    },
    createClass(
      { commit }: { commit: Function },
      payload: {
        className: string
        description: string
      }
    ): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/create`,
        data: {
          className: payload.className,
          description: payload.description,
        },
      })
        .then(() => {
          commit('setClassErrMsg', '')
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setClassErrMsg', err.response.data)
        })
    },
    // eslint-disable-next-line
    deleteClass({ commit }: { commit: Function }, classCode: string): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/remove`,
        data: {
          classCode: classCode,
        },
      })
        .then(() => {
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    },
    getAssignedDecksAsStudent(
      { commit }: { commit: Function },
      classCode: string
    ): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/getAssignedDecksAsStudent`,
        data: {
          classCode: classCode,
        },
      })
        .then((res) => {
          commit('setCurrClassAssignments', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setCurrClassAssignments', [])
        })
    },
    getAssignedDecksAsTeacher(
      { commit }: { commit: Function },
      classCode: string
    ): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/getAssignedDecksAsTeacher`,
        data: {
          classCode: classCode,
        },
      })
        .then((res) => {
          commit('setCurrClassAssignments', res.data)
        })
        .catch((err) => {
          console.log(err)
          commit('setCurrClassAssignments', [])
        })
    },
    // eslint-disable-next-line
    assignDeck({ commit }: { commit: Function }, payload: any): void { // TODO: Specify payload type
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/assign`,
        data: payload,
      })
        .then(() => {
          commit('setClassErrMsg', '')
          router.push({ name: 'Classes' })
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setClassErrMsg', err.response.data)
        })
    },
    unassignDeck({ commit }: { commit: Function }, payload: any): void { // TODO: Specify payload type
    // let teacher = req.user._id
    // let classCode = req.body.classCode
    // let deckId = req.body.deckId
    // let assignmentId = req.body.assignmentId
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/unassign`,
        data: payload,
      })
        .then(() => {
          console.log('hi')
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setClassErrMsg', err.response.data)
        })
    },
  },
}

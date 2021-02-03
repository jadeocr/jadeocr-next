import router from '../../router/index'
import axios from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL
import { ClassI } from '../../interfaces/Class'
import { Deck } from '../../interfaces/Deck'

export const classes = {
  namespaced: true,
  state: {
    classErrMsg: '',
    classes: Array<ClassI>(),
    classesTeaching: Array<ClassI>(),
    currClassAssignedDecks: Array<Deck>(),
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
    setCurrClassAssignedDecks(state: any, decks: Array<Deck>) {
      state.currClassAssignedDecks = decks
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
          commit('setCurrClassAssignedDecks', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setCurrClassAssignedDecks', [])
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
          commit('setCurrClassAssignedDecks', res.data)
        })
        .catch((err) => {
          console.log(err)
          commit('setCurrClassAssignedDecks', [])
        })
    },
    // eslint-disable-next-line
    assignDeck({ commit }: { commit: Function }, payload: any): void {
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
  },
}

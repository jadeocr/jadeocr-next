import router from '../../router/index'
import axios from 'axios'
import { AxiosResponse } from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL
import { ClassI } from '../../interfaces/Class'
import { AssignedDeck } from '../../interfaces/AssignedDeck'
import { UnassignDeckPayload } from '../../interfaces/UnassignDeckPayload'
import { AssignDeckData } from '../../interfaces/AssignDeckData'

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
    async getClassesTeaching({
      commit,
      rootState,
    }: {
      commit: Function
      // eslint-disable-next-line
      rootState: any
    }): Promise<any> {
      if (rootState.auth.isTeacher) {
        return axios({
          method: 'post',
          withCredentials: true,
          url: `${apiBaseURL}/class/getTeachingClasses`,
        })
          .then((res: AxiosResponse) => {
            commit('setClassesTeaching', res.data)
          })
          .catch((err) => {
            console.log(err.response.data)
          })
      } else {
        return -1
      }
    },
    async getClassesJoined({ commit }: { commit: Function }): Promise<any> {
      return axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/getJoinedClasses`,
      })
        .then((res: AxiosResponse) => {
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
    async getAssignedDecksAsStudent(
      { commit }: { commit: Function },
      classCode: string
    ): Promise<any> {
      return axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/getAssignedDecksAsStudent`,
        data: {
          classCode: classCode,
        },
      })
        .then((res: AxiosResponse) => {
          commit('setCurrClassAssignments', res.data)
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setCurrClassAssignments', [])
        })
    },
    async getAssignedDecksAsTeacher(
      { commit }: { commit: Function },
      classCode: string
    ): Promise<any> {
      return axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/getAssignedDecksAsTeacher`,
        data: {
          classCode: classCode,
        },
      })
        .then((res: AxiosResponse) => {
          commit('setCurrClassAssignments', res.data)
        })
        .catch((err) => {
          console.log(err)
          commit('setCurrClassAssignments', [])
        })
    },
    // eslint-disable-next-line
    assignDeck({ commit }: { commit: Function }, payload: AssignDeckData): void {
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
    unassignDeck(
      { commit }: { commit: Function },
      payload: UnassignDeckPayload
    ): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/unassign`,
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

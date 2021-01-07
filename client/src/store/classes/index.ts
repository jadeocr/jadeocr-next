import router from '../../router/index'
import axios from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL
import { ClassI } from '../../interfaces/Class'

export const classes = {
  namespaced: true,
  state: {
    classErrMsg: '',
    classes: Array<ClassI>(),
    classesTeaching: Array<ClassI>(),
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
  },
  actions: {
    getClassesTeaching({
      commit,
      rootState,
    }: {
      commit: Function
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
          router.push({ name: 'Classes' })
        })
        .catch((err) => {
          console.log(err.response.data)
          commit('setClassErrMsg', err.response.data)
        })
    },
    deleteClass({ commit }: { commit: Function }, classCode: string): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/remove`,
        data: {
          classCode: classCode
        }
      })
        .then(() => {
          router.push({ name: 'Classes' })
        })
        .catch((err) => {
          console.log(err.response.data)
        })
    }
  },
}

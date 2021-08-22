import router from '../../router/index'
import axios from 'axios'
import { AxiosResponse } from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL
import { User } from '../../interfaces/User'

export const auth = {
  namespaced: true,
  state: {
    isSignedIn: false,
    authErrorMsg: '',
    firstName: '',
    lastName: '',
    email: '',
    uid: '',
    isTeacher: false,
    stats: {},
  },
  mutations: {
    // eslint-disable-next-line
    setAuthErrorMsg(state: any, msg: string) {
      state.authErrorMsg = msg
    },
    // eslint-disable-next-line
    setSignedIn(state: any, isSignedIn: boolean) {
      state.isSignedIn = isSignedIn
    },
    // eslint-disable-next-line
    setFirstName(state: any, firstName: string) {
      state.firstName = firstName
    },
    // eslint-disable-next-line
    setLastName(state: any, lastName: string) {
      state.lastName = lastName
    },
    // eslint-disable-next-line
    setEmail(state: any, email: string) {
      state.email = email
    },
    // eslint-disable-next-line
    setUID(state: any, uid: string) {
      state.uid = uid
    },
    // eslint-disable-next-line
    setTeacher(state: any, isTeacher: boolean) {
      state.isTeacher = isTeacher
    },
    // eslint-disable-next-line
    setStats(state: any, stats: string) {
      state.stats = stats
    },
  },
  actions: {
    signUp({ commit }: { commit: Function }, credentials: object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signup`,
        data: credentials,
        withCredentials: true,
      }).catch(() => {
        commit('setAuthErrorMsg', 'User already exists')
      })
    },
    signIn({ commit }: { commit: Function }, credentials: object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signin`,
        data: credentials,
        withCredentials: true,
      })
        .then(() => {
          axios({
            method: 'get',
            url: `${apiBaseURL}/user`,
            withCredentials: true,
          })
            .then((res: AxiosResponse<User>) => {
              commit('setSignedIn', true)
              commit('setUID', res.data._id)
              commit('setEmail', res.data.email)
              commit('setFirstName', res.data.firstName)
              commit('setLastName', res.data.lastName)
              commit('setTeacher', res.data.isTeacher)
              router.push({ name: 'Dashboard' })
            })
            .catch((err) => {
              console.log(err)
            })
        })
        .catch((err) => {
          if (err.response.status == 401) {
            commit('setAuthErrorMsg', 'Invalid login')
          }
        })
    },
    autoSignIn({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/user`,
        withCredentials: true,
      })
        .then((res: AxiosResponse<User>) => {
          commit('setSignedIn', true)
          commit('setUID', res.data._id)
          commit('setEmail', res.data.email)
          commit('setFirstName', res.data.firstName)
          commit('setLastName', res.data.lastName)
          commit('setTeacher', res.data.isTeacher)
          router.push({ name: 'Dashboard' })
        })
        .catch(() => {
          return // cookie doesn't exist, can just return
        })
    },
    signOut({ commit }: { commit: Function }): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signout`,
        withCredentials: true,
      })
        .then(() => {
          commit('setSignedIn', false)
          router.push({ name: 'Home' })
        })
        .catch((err) => {
          console.log(err)
        })
    },
    getDetails({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/user/details`,
        withCredentials: true,
      })
        .then((res: AxiosResponse) => {
          console.log(res)
        })
        .catch((err) => {
          console.log(err)
        })
    },
    getStats({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        url: `${apiBaseURL}/user/stats`,
        withCredentials: true,
      })
        .then((res: AxiosResponse) => {
          commit('setStats', res.data)
        })
        .catch((err) => {
          console.log(err)
        })
    },
    toggleTeacher(
      { dispatch }: { dispatch: Function },
      isTeacher: boolean
    ): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/setTeacher`,
        withCredentials: true,
        data: {
          isTeacher: isTeacher,
        },
      })
        .then((res: AxiosResponse) => {
          dispatch('autoSignIn')
        })
        .catch((err) => {
          console.log(err)
        })
    },
  },
}

import router from '../../router/index'
import axios from 'axios'

const apiBaseURL = 'http://localhost:3000/api'

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
    }
  },
  actions: {
    signUp({ commit }: { commit: Function }, credentials: object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signup`,
        data: credentials,
        withCredentials: true,
      })
        .catch(() => {
          commit('setAuthErrorMsg', 'User already exists')
        })
    },
    signIn({ commit }: { commit: Function }, credentials: object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signin`,
        data: credentials,
        withCredentials: true
      })
        .then((res) => {
          commit('setSignedIn', true)
          commit('setUID', res.config.data._id)
          commit('setEmail', res.config.data.email)
          commit('setFirstName', res.config.data.firstName)
          commit('setLastName', res.config.data.lastName)
          commit('setTeacher', res.config.data.isTeacher)
          router.push({ name: 'Dashboard' })
        })
        .catch((err) => {
          if (err.response.status == 401) {
            commit('setAuthErrorMsg', 'Invalid login')
          }
        })
    },
  },
}

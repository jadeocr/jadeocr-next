import router from '../../router/index'
import axios from 'axios'

const apiBaseURL = 'http://localhost:3000/api'

export const auth = {
  namespaced: true,
  state: {
    isSignedIn: false,
    authErrorMsg: '',
    email: '',
  },
  mutations: {
    // eslint-disable-next-line
    setAuthErrorMsg(state: any, msg: string) {
      state.authErrorMsg = msg
    },
    // eslint-disable-next-line
    setSignedInState(state: any, value: boolean) {
      state.isSignedIn = value
    },
    // eslint-disable-next-line
    setEmail(state: any, email: string) {
      state.email = email
    },
  },
  actions: {
    signUp({ commit }: { commit: Function }, credentials: object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signup`,
        data: credentials,
      })
        .then((res) => console.log(res)) // TODO: Response handling
        .catch((err) => {
          commit('setAuthErrorMsg', 'User already exists')
          console.log(err)
        })
    },
    signIn({ commit }: { commit: Function }, credentials: object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signin`,
        data: credentials,
      })
        .then((res) => {
          commit('setSignedInState', true)
          commit('setEmail', res.config.data.email)
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

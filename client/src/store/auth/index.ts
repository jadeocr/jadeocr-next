import axios from 'axios'

const apiBaseURL = 'http://localhost:3000/api'

export const auth = {
  namespaced: true,
  state: {
    isSignedIn: false,
  },
  mutations: {},
  actions: {
    signUp({ commit }: { commit: Function }, credentials:object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signup`,
        data: credentials,
      })
        .then((res) => console.log(res)) // TODO: Response handling
        .catch((err) => console.log(err))
    },
    signIn({ commit }: { commit: Function }, credentials:object): void {
      axios({
        method: 'post',
        url: `${apiBaseURL}/signin`,
        data: credentials,
      })
        .then((res) => console.log(res)) // TODO: Response handling
        .catch((err) => console.log(err))
    }
  },
}

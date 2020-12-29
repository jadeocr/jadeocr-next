import router from '../../router/index'
import axios from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL

export const classes = {
  namespaced: true,
  state: {
    classErrMsg: '',
  },
  mutations: {
    // eslint-disable-next-line
    setClassErrMsg(state: any, msg: string) {
      state.classErrMsg = msg
    }
  },
  actions: {
    getClasses(): void {
      axios({
        method: 'get',
        withCredentials: true,
        url: `${apiBaseURL}/user/details`,
      })
    },
    joinClass({ commit }: { commit: Function }, classCode: number): void {
      axios({
        method: 'post',
        withCredentials: true,
        url: `${apiBaseURL}/class/join`,
        data: {
          classCode: classCode
        }
      })
        .then(() => {
          commit('setClassErrMsg', '')
        })
        .catch((err) => {
          commit('setClassErrMsg', err.response.data)
          console.log(err.response.data)
        })
    }
  },
}

import router from '../../router/index'
import axios from 'axios'
const apiBaseURL = process.env.VUE_APP_API_BASEURL

export const classes = {
  namespaced: true,
  state: {
    classErrMsg: '',
    classes: Array<string>(),
    classesTeaching: Array<string>(),
  },
  mutations: {
    // eslint-disable-next-line
    setClassErrMsg(state: any, msg: string) {
      state.classErrMsg = msg
    },
    // eslint-disable-next-line
    setClasses(state: any, classes: Array<string>) {
      state.classes = classes
    },
    // eslint-disable-next-line
    setClassesTeaching(state: any, classesTeaching: Array<string>) {
      state.classesTeaching = classesTeaching
    },
  },
  actions: {
    getClasses({ commit }: { commit: Function }): void {
      axios({
        method: 'get',
        withCredentials: true,
        url: `${apiBaseURL}/user/details`,
      })
        .then((res) => {
          commit('setClasses', res.data.classes)
          commit('setClassesTeaching', res.data.classesTeaching)
        })
        .catch((err) => {
          console.log(err)
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

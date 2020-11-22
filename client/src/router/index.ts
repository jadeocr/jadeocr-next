import { createRouter, createWebHistory, RouteRecordRaw } from 'vue-router'
import store from '../store/index'
import Home from '../views/Home.vue'

const routes: Array<RouteRecordRaw> = [
  {
    path: '/',
    name: 'Home',
    component: Home,
  },
  {
    path: '/login',
    name: 'Login',
    component: () =>
      import(/* webpackChunkName: 'login' */ '../views/Login.vue'),
    beforeEnter: () => {
      store.dispatch('auth/autoSignIn')
    },
  },
  {
    path: '/signup',
    name: 'Signup',
    component: () =>
      import(/* webpackChunkNmae: 'signup' */ '../views/Signup.vue'),
  },
  {
    path: '/dashboard',
    name: 'Dashboard',
    component: () =>
      import(/* webpackChunkName: 'dashboard' */ '../views/Dashboard.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/deckedit/create',
    name: 'DeckEdit',
    component: () =>
      import(/* webpackChunkName: 'deckedit' */ '../views/DeckEdit.vue'),
  },
  {
    path: '/deckedit/edit/:name',
    name: 'DeckEdit',
    props: true,
    component: () =>
      import(/* webpackChunkName: 'deckedit' */ '../views/DeckEdit.vue'),
  },
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes,
})

// eslint-disable-next-line
const state:any = store.state

router.beforeEach((to, from, next): void => {
  const requiresAuth = to.matched.some((record) => record.meta.requiresAuth)
  if (requiresAuth && !state.auth.isSignedIn) {
    next('login')
  } else {
    next()
  }
})

export default router

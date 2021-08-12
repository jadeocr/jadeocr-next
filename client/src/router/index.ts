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
      import(/* webpackChunkName: 'signup' */ '../views/Signup.vue'),
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
    name: 'DeckCreate',
    component: () =>
      import(/* webpackChunkName: 'deckedit' */ '../views/DeckEdit.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/deckedit/edit/:id',
    name: 'DeckEdit',
    props: true,
    component: () =>
      import(/* webpackChunkName: 'deckedit' */ '../views/DeckEdit.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/classes',
    name: 'Classes',
    component: () =>
      import(/* webpackChunkName: 'classes' */ '../views/Classes.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/deck/:id',
    props: true,
    name: 'Deck',
    component: () => import(/* webpackChunkName: 'deck' */ '../views/Deck.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/review/:type/:id/:title',
    props: true,
    name: 'Review',
    component: () =>
      import(/* webpackChunkName: 'review' */ '../views/Review.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/class/:classCode',
    props: true,
    name: 'Class',
    component: () =>
      import(/* webpackChunkName: 'class' */ '../views/Class.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/class/:classCode/edit',
    props: true,
    name: 'ClassEdit',
    component: () =>
      import(/* webpackChunkName: 'classedit' */ '../views/ClassEdit.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/profile',
    name: 'Profile',
    component: () =>
      import(/* webpackChunkName: 'profile' */ '../views/Profile.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/public',
    props: true,
    name: 'Public',
    component: () =>
      import(/* webpackChunkName: 'public' */ '../views/Public.vue'),
    meta: {
      requiresAuth: true,
    },
  },
  {
    path: '/terms',
    name: 'Terms',
    component: () =>
      import(/* webpackChunkName: 'terms' */ '../views/Terms.vue'),
  },
  {
    path: '/:catchAll(.*)',
    name: 'NotFound',
    component: () =>
      import(/* webpackChunkName: 'notfound' */ '../views/NotFound.vue'),
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

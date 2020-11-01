<template>
  <div id="auth-form">
    <div
      class="w-4/5 px-8 py-8 mx-auto mt-10 rounded bg-nord0 md:w-1/2 xl:w-1/4 lg:w-1/3"
    >
      <!-- TODO: Add submit action -->
      <form @submit.prevent="">
        <div class="my-4">
          <label for="email" class="block mb-1">Email</label>
          <input
            class="w-full px-3 py-1 rounded text-nord0 bg-nord4"
            v-model="credentials.email"
            placeholder="alice@example.com"
            type="text"
          />
        </div>
        <div class="my-6">
          <label for="password" class="block mb-1">Password</label>
          <input
            class="w-full px-3 py-1 rounded text-nord0 bg-nord4"
            v-model="credentials.password"
            placeholder="••••••••"
            type="password"
          />
        </div>
        <!-- Signup page only -->
        <div v-if="formType == 'signup'" class="my-6">
          <label for="password" class="block mb-1">Confirm Password</label>
          <input
            class="w-full px-3 py-1 text-gray-800 rounded bg-nord4"
            v-model="credentials.confirmPassword"
            placeholder="••••••••"
            type="password"
          />
        </div>
      </form>
      <!-- Login page buttons -->
      <div v-if="formType == 'login'">
        <div class="flex items-center justify-between mt-8">
          <button class="px-4 py-2 rounded bg-nord3" @click="callSignIn()">
            Log In
          </button>
          <router-link :to="{ name: 'Signup' }" class="px-4 py-2">
            Sign Up
          </router-link>
        </div>
      </div>
      <!-- Signup page buttons -->
      <div v-else>
        <div class="flex items-center justify-between mt-8">
          <button class="px-4 py-2 rounded bg-nord3">
            Sign Up
          </button>
          <router-link :to="{ name: 'Login' }" class="px-4 py-2">
            Login
          </router-link>
        </div>
      </div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import axios from 'axios'

  interface Credentials {
    email: string
    password: string
    confirmPassword: string
  }

  export default defineComponent({
    name: 'Login',
    props: {
      formType: {
        type: String,
        required: true,
        validator: (value: string): boolean => {
          return ['login', 'signup'].includes(value)
        },
      },
    },
    setup() {
      return {
        credentials: {
          email: '',
          password: '',
          confirmPassword: '',
        } as Credentials,
      }
    },
    methods: {
      validateEmail(email: string): boolean {
        const re = /^(([^<>()[\]\\.,;:\s@']+(\.[^<>()[\]\\.,;:\s@']+)*)|('.+'))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,4}))$/
        return re.test(email)
      },
      validatePasswordLength(password: string): boolean {
        return password.length >= 8
      },
      callSignIn() {
        axios({
          method: 'post',
          url: 'http://localhost:3000/api/signup', // TODO: Update to db url
        })
      },
    },
  })
</script>

<style scoped></style>

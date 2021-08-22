<template>
  <div id="auth-form">
    <div
      class="max-w-xs p-12 mx-auto mt-10 md:max-w-xl md:p-20 rounded-md bg-nord0"
    >
      <form @submit.prevent="">
        <div class="my-4 grid grid-cols-2" v-if="formType == 'signup'">
          <div class="mr-2 col-span-1">
            <label for="firstName" class="block mb-1">First Name</label>
            <input
              class="w-full px-4 py-2 rounded text-nord0 bg-nord4"
              v-model="credentials.firstName"
              placeholder="Ada"
              type="text"
            />
          </div>
          <div class="ml-2 col-span-1">
            <label for="lastName" class="block mb-1">Last Name</label>
            <input
              class="w-full px-4 py-2 rounded text-nord0 bg-nord4"
              v-model="credentials.lastName"
              placeholder="Lovelace"
              type="text"
            />
          </div>
        </div>
        <!-- Login page only -->
        <div class="my-6">
          <label for="email" class="block mb-1">Email</label>
          <input
            class="w-full px-4 py-2 rounded text-nord0 bg-nord4"
            v-model="credentials.email"
            placeholder="ada@example.com"
            type="email"
          />
        </div>
        <div class="my-6">
          <label for="password" class="block mb-1">Password</label>
          <input
            class="w-full px-4 py-2 rounded text-nord0 bg-nord4"
            v-model="credentials.password"
            placeholder="••••••••••••••••"
            type="password"
          />
        </div>
        <!-- Signup page only -->
        <div v-if="formType == 'signup'" class="my-6">
          <label for="password" class="block mb-1">Confirm Password</label>
          <input
            class="w-full px-4 py-2 text-gray-800 rounded bg-nord4"
            v-model="credentials.confirmPassword"
            placeholder="••••••••••••••••"
            type="password"
          />
        </div>
        <div
          v-if="errorMsg || $store.state.auth.authErrorMsg"
          class="text-nord11"
        >
          {{ errorMsg }}
          {{ $store.state.auth.authErrorMsg }}
        </div>
        <!-- Login page buttons -->
        <div v-if="formType == 'login'">
          <div class="flex items-center justify-between mt-8">
            <button
              class="px-4 py-2 rounded bg-nord3 hoverlink"
              @click="callSignIn()"
            >
              Log In
            </button>
            <router-link
              :to="{ name: 'Signup' }"
              class="px-4 py-2 font-normal hoverlink"
            >
              Sign Up
            </router-link>
          </div>
        </div>
        <!-- Signup page buttons -->
        <div v-else>
          <div class="flex items-center justify-between mt-8">
            <button
              class="px-4 py-2 rounded bg-nord3 hoverlink"
              @click="callSignUp()"
            >
              Sign Up
            </button>
            <router-link
              :to="{ name: 'Login' }"
              class="px-4 py-2 font-normal hoverlink"
            >
              Login
            </router-link>
          </div>
        </div>
      </form>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'

  interface Credentials {
    firstName: string
    lastName: string
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
    data() {
      return {
        credentials: {
          firstName: '',
          lastName: '',
          email: '',
          password: '',
          confirmPassword: '',
        } as Credentials,
        errorMsg: '',
      }
    },
    methods: {
      validateEmail(email: string): boolean {
        const re = /^(([^<>()[\]\\.,;:\s@']+(\.[^<>()[\]\\.,;:\s@']+)*)|('.+'))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,4}))$/
        return re.test(email)
      },
      validatePassword(password: string, confirmPassword: string): boolean {
        return password == confirmPassword
      },
      clearFields(): void {
        this.credentials = {
          firstName: '',
          lastName: '',
          email: '',
          password: '',
          confirmPassword: '',
        }
      },
      callSignUp(): void {
        this.$store.commit('auth/setAuthErrorMsg', '')
        if (
          this.validatePassword(
            this.credentials.password,
            this.credentials.confirmPassword
          ) &&
          this.validateEmail(this.credentials.email)
        ) {
          this.errorMsg = ''
          this.$store.dispatch('auth/signUp', this.credentials)
        } else if (!this.validateEmail(this.credentials.email)) {
          this.errorMsg = 'Invalid email'
        } else {
          this.errorMsg = 'Password confirmation does not match'
        }
        this.clearFields()
      },
      callSignIn() {
        this.$store.commit('auth/setAuthErrorMsg', '')
        if (this.validateEmail(this.credentials.email)) {
          this.errorMsg = ''
          this.$store.dispatch('auth/signIn', this.credentials)
        } else {
          this.errorMsg = 'Invalid email'
        }
        this.clearFields()
      },
    },
  })
</script>

<style scoped></style>

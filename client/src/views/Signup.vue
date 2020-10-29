<template>
  <div id='signup'>
    <auth-form formType='signup'/>
  </div>	
</template>

<script lang="ts">
import { defineComponent } from 'vue'
import AuthForm from '../components/AuthForm.vue'

export default defineComponent({
  name: 'Signup',
  components: {
    AuthForm
  },
  data(): object {
    return {
      email: '',
      password: '',
      confirmPassword: ''
    }
  }
})
</script>

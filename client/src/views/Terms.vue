<template>
  <div class="m-4 Terms md:m-8 lg:m-12">
    <h1 class="my-4 text-2xl md:text-3xl lg:text-4xl"> Terms of Use and Privacy Policy </h1>
    We do everything here in good faith, and we hope you will, too. These terms are in place for your protection and for ours. By using jadeocr, you agree to the following terms.
    <h2 class="mt-6 mb-2 text-xl md:text-2xl lg:text-3xl"> Terms of Use </h2>
    <ol class="mx-4 list-decimal list-inside"> 
      <li> 
        This software is licensed under the <router-link to="/terms" target="_blank" class="hoverlink"> MIT License</router-link>. Thus, all terms of the MIT License apply to these terms of use. 
      </li>
      <li> 
        You will not use this software to cause harm to another individual or individuals. (Just in case someone, somehow, figures out a way to.)
      </li>
      <li>
        You agree to the privacy policy below.
      </li>
      <li> 
        Jadeocr's maintainers reserve the right to terminate your account at any time.
      </li>
      <li> 
        If you choose not to use the webapp linked in the <b> README.md </b> in the <a class="font-bold hover:opacity-50" href="https://github.com/jadeocr/jadeocr-next">jadeocr-next</a> repository, terms 3 and 4 do not apply. You're on your own. In this case, do whatever you want as long as it complies with terms 1 and 2 (which isn't exactly hard to do).
      </li>
    </ol>
    
    <h2 class="mt-6 mb-2 text-xl md:text-2xl lg:text-3xl"> Privacy Policy </h2>
    <ol class="mx-4 list-decimal list-inside"> 
      <li> 
        Jadeocr uses its own server for handling user authentication, storing user data, and processing handwriting.
        When you sign in with email and password, jadeocr stores your email address and a hashed password.
      </li>
      <li> 
        All data is stored unencrypted in a MongoDB database, which only the maintainers have access to.
      </li>
      <li>
        Only decks that are explicitly made public are displayed in the "Public Decks" tab.
        All other decks are only visible to the creator and classes that the creator has
        assigned decks to.
      </li>
      <li> 
        Jadeocr's maintainers have full access to the database.
      </li>
      <li> 
        Backups of the database will also be regularly made at the maintainers' discretion.
      </li>
      <li> 
        Jadeocr only uses cookies to persist user data between page refreshes.
      </li>
      <li> 
        Our minimalist philosophy extends behind the scenes, too! If we don't need it—especially when it involves your privacy—we won't use it. We have nothing to gain from your data, anyway, and it's easier to respect your privacy than to protect it.
      </li>
    </ol>
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue'

  export default defineComponent({
    name: 'Terms',
  })
</script>


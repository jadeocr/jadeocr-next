<template>
  <div id="navigation" class="w-1/2 mx-auto mt-8 text-center">
    <nav class="text-2xl md:text-3xl xl:text-4xl">
      <router-link :to='{ name: navRoute }'>jadeocr</router-link>
    </nav>
  </div>
  <router-view/>
</template>


<style>
@import './assets/css/tailwind.css';
@import url('https://fonts.googleapis.com/css2?family=Open+Sans:wght@300;400;600&display=swap');

body {
  background-size: cover;
  background: #121212;
}
#app {
	-webkit-font-smoothing: antialiased;
	-moz-osx-font-smoothing: grayscale;
	font-family: 'Open Sans', sans-serif;
	font-weight: 300;
	color: #ffffff;
}


/* Scrollbar */
/* Credit to @devinrhode2 on GitHub: https://gist.github.com/devinrhode2/2573411 */
::-webkit-scrollbar {
  width: 8px;
  background-color: rgba(0,0,0,0);
  -webkit-border-radius: 100px;
	border-radius: 100px;
}

::-webkit-scrollbar:hover {
  background-color: rgba(255,255,255, 0.05);
  transition: background-color 0.5 ease;
}

::-webkit-scrollbar-thumb:vertical {
  background: rgba(255,255,255,0.3);
  -webkit-border-radius: 100px;
	border-radius: 100px;
}
::-webkit-scrollbar-thumb:vertical:active {
  background: rgba(255,255,255,0.2);
  -webkit-border-radius: 100px;
	border-radius: 100px;
}

.page-content {
	-webkit-transition: width 0.3s ease, opacity 0.1s ease, visibility 0.1s ease;
	transition: width 0.3s ease, opacity 0.1s ease, visibility 0.1s ease;
}
</style>

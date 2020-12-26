<template>
  <div id="review">
    <div
      class="w-3/5 m-auto mt-8 mb-2 text-xl font-normal text-center rounded md:mt-16 lg:text-left lg:w-1/2 lg:text-2xl xl:text-3xl md:text-3xl"
    >
      {{ title }}
    </div>
    <div class="w-3/5 mx-auto mt-8 text-center lg:w-1/2">
      <div class="card-container">
        <div
          class="px-6 py-12 text-xl font-normal rounded bg-nord0 card lg:text-2xl xl:text-3xl md:text-3xl"
        >
          <div>
            <p
              v-for="(n, side) in visibleCardData"
              :key="side.key"
              class="font-light chinese"
            >
              {{ visibleCardData[side] }}
            </p>
          </div>
        </div>
        <div
          v-if="type == 'ocr' || type == 'quiz'"
          id="draw-wrapper"
          ref="draw-wrapper"
          class="mt-8 lg:ml-10 lg:mt-0"
        >
          <canvas id="draw"></canvas>
          <div
            class="flex justify-end mt-1 mr-0 md:mt-5 lg:mt-2"
            id="canvas-ctrls"
          >
            <div id="clearbtn" class="mx-2" @click="clearCanvas()">
              <svg
                class="bi bi-arrow-counterclockwise"
                width="1.25em"
                height="1.25em"
                viewBox="0 0 16 16"
                fill="currentColor"
                xmlns="http://www.w3.org/2000/svg"
              >
                <path
                  fill-rule="evenodd"
                  d="M12.83 6.706a5 5 0 0 0-7.103-3.16.5.5 0 1 1-.454-.892A6 6 0 1 1 2.545 5.5a.5.5 0 1 1 .91.417 5 5 0 1 0 9.375.789z"
                />
                <path
                  fill-rule="evenodd"
                  d="M7.854.146a.5.5 0 0 0-.708 0l-2.5 2.5a.5.5 0 0 0 0 .708l2.5 2.5a.5.5 0 1 0 .708-.708L5.707 3 7.854.854a.5.5 0 0 0 0-.708z"
                />
              </svg>
            </div>
          </div>
        </div>
      </div>
      <div class="mt-10">
        <div
          class="flex items-center justify-between m-auto mt-4 md:w-2/3 opacity-87"
        >
          <div
            class="px-4 py-3 btn bg-nord12 rounded-md"
            @click="type != 'quiz' ? cardCheck('incorrect') : override('incorrect')"
          >
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.1em"
              fill="currentColor"
              class="bi bi-x"
              viewBox="0 0 16 16"
            >
              <path
                fill-rule="evenodd"
                d="M4.646 4.646a.5.5 0 0 1 .708 0L8 7.293l2.646-2.647a.5.5 0 0 1 .708.708L8.707 8l2.647 2.646a.5.5 0 0 1-.708.708L8 8.707l-2.646 2.647a.5.5 0 0 1-.708-.708L7.293 8 4.646 5.354a.5.5 0 0 1 0-.708z"
              />
            </svg>
          </div>
          <div
            class="px-4 py-2 mx-auto font-normal text-md rounded-md md:text-lg unselect bg-nord9"
            @click="flipCard()"
          >
            {{ type == 'flashcards' ? 'Flip Card' : 'Check Writing' }}
          </div>
          <div
            class="px-4 py-3 rounded-md bg-nord7"
            @click="type != 'quiz' ? cardCheck('correct') : override('correct')"
          >
            <svg
              xmlns="http://www.w3.org/2000/svg"
              width="1.1em"
              fill="currentColor"
              class="bi bi-check2"
              viewBox="0 0 16 16"
            >
              <path
                fill-rule="evenodd"
                d="M13.854 3.646a.5.5 0 0 1 0 .708l-7 7a.5.5 0 0 1-.708 0l-3.5-3.5a.5.5 0 1 1 .708-.708L6.5 10.293l6.646-6.647a.5.5 0 0 1 .708 0z"
              />
            </svg>
          </div>
        </div>
      </div>
			<div v-if="pred"
			class="mt-4 text-lg text-teal-500 md:mt-8 md:text-xl chinese">
				{{ pred }}
			</div>
    </div>
  </div>
</template>

<script lang="ts">
  import { defineComponent } from 'vue'
  import { ReviewResult } from '../interfaces/ReviewResult'
  import { Character } from '../interfaces/Character'
  import axios from 'axios'
  const apiBaseURL = process.env.VUE_APP_API_BASEURL

  export default defineComponent({
    name: 'Review',
    props: {
      type: {
        type: String,
        required: true,
        validator: (value: string): boolean => {
          return ['flashcards', 'ocr', 'quiz'].includes(value)
        },
      },
      id: {
        type: String,
        required: true,
      },
      title: {
        type: String,
        required: true,
      },
    },
    data() {
      return {
        cards: Array<Character>(),
        // eslint-disable-next-line
        visibleCardData: [] as any,
        currReviewIndex: 0,
        results: Array<ReviewResult>(),
        // Mouse movement tracker
        xPos: 0,
        yPos: 0,
        subStrokeArray: [] as any,
        strokeArray: [] as any,
        mouseState: 'up',
        pred: '',
        currTime: Date.now(),
      }
    },
    methods: {
      cardCheck(correctness: string): void {
        this.results.push({
          id: this.cards[this.currReviewIndex].id,
          quality: correctness == 'correct' ? 5 : 0,
        } as ReviewResult)
        if (this.currReviewIndex + 1 == this.cards.length) {
          this.sendResults()
        } else {
          this.currReviewIndex++
          this.clearCanvas()
          this.visibleCardData = [
            this.cards[this.currReviewIndex].pinyin,
            this.cards[this.currReviewIndex].definition,
          ]
        }
      },
      override(correctness: string): void {
        this.results[this.results.length - 1].quality = correctness == 'correct' ? 5 : 0
        this.pred = `Marked previous card (${this.cards[this.currReviewIndex - 1].char}) as ${correctness}`
      },
      resetVisibleCard(): void {
        this.visibleCardData = [
          this.cards[this.currReviewIndex].pinyin,
          this.cards[this.currReviewIndex].definition,
        ]
      },
      callOcr(): void {
        this.pred = 'Checking...'
        axios({
          method: 'post',
          withCredentials: true,
          url: `${apiBaseURL}/ocr`,
          data: {
            strokes: JSON.stringify(this.strokeArray),
          },
        })
          .then((res) => {
            const currCard = this.cards[this.currReviewIndex].char
            if (this.type == 'quiz') {
              const correctness = res.data[0] == currCard ? 'correct' : 'incorrect'
              this.cardCheck(correctness)
              this.pred = correctness == 'correct' ? 'Correct! ' : 'Incorrect '
            } else {
              this.pred = res.data[0] == currCard ? 'Correct! ' : 'Try again! '
              this.pred += `You wrote ${res.data[0]}`
            }
          })
          .catch((err) => {
            console.log(err)
          })
      },
      flipCard(): void {
        if (this.type == 'ocr' || this.type == 'quiz') {
          this.callOcr()
        }
        if (this.visibleCardData.length == 3) { // If showing all sides of card
          this.resetVisibleCard()
        } else {
          this.visibleCardData.push(this.cards[this.currReviewIndex].char)
        }
      },
      sendResults(): void {
        this.$store.dispatch('decks/sendReviewResults', {
          deckId: this.id,
          results: this.results,
        })
      },
      getCardsToReview(): void {
        if (this.type == 'quiz') {
          this.cards = this.$store.state.decks.currDeck.characters
          this.resetVisibleCard()
        } else {
          axios({
            method: 'post',
            url: `${apiBaseURL}/deck/srs`,
            withCredentials: true,
            data: {
              deckId: this.id,
            },
          })
            .then((res) => {
              this.cards = res.data
              this.resetVisibleCard()
            })
            .catch((err) => {
              console.log(err.response.data)
            })
        }
      },
      setPos(e: MouseEvent) {
        // repeated in various functions to bypass typescript issue
        const canvas = document.getElementById('draw') as HTMLCanvasElement
        const domRect = canvas.getBoundingClientRect()
        this.xPos = (e.clientX - domRect.left) / domRect.width * canvas.width
        this.yPos = (e.clientY - domRect.top) / domRect.height * canvas.height
      },
      drawLine(e: MouseEvent, ctx: any) {
        if (!window.matchMedia("(pointer: coarse)").matches) {
          if (e.buttons !== 1) return // is not touchscreen
        }
        ctx.beginPath()
        ctx.lineWidth = 5
        ctx.strokeStyle = '#ffffff'
        ctx.lineCap = 'round'
        ctx.moveTo(this.xPos, this.yPos)
        this.setPos(e)
        ctx.lineTo(this.xPos, this.yPos)
        ctx.stroke()
      },
      mouseDown(e: MouseEvent) {
        this.mouseState = 'down'
        this.setPos(e)
      },
      mouseUp(e: MouseEvent) {
        const canvas = document.getElementById('draw') as HTMLCanvasElement
        const ctx = canvas?.getContext('2d')
        if (this.mouseState == 'down') {
          this.drawLine(e, ctx)
          this.xPos = 0
          this.yPos = 0
          this.mouseState = 'up'
          this.strokeArray.push(this.subStrokeArray)
          this.subStrokeArray = []
        }
      },
      move(e: MouseEvent) {
        const canvas = document.getElementById('draw') as HTMLCanvasElement
        const ctx = canvas?.getContext('2d')
        if (this.mouseState == 'down') {
          this.drawLine(e, ctx)
          this.setPos(e)

          if ((Date.now() - this.currTime) > 20) {
            this.currTime = Date.now()
            this.subStrokeArray.push([this.xPos, this.yPos])
          }
        }
      },
      clearCanvas() {
        const canvas = document.getElementById('draw') as HTMLCanvasElement
        const ctx = canvas?.getContext('2d')
        this.pred = ''
        if (ctx) {
          ctx.clearRect(0, 0, canvas.width, canvas.height)
          this.strokeArray = []
        }
      }
    },
    mounted() {
      this.getCardsToReview()
    },
    updated() {
      if (this.type == 'ocr' || this.type == 'quiz') {
        const canvas = document.getElementById('draw') as HTMLCanvasElement
        canvas?.addEventListener('pointermove', this.move, false)
        canvas?.addEventListener('pointerdown', this.mouseDown, false)
        canvas?.addEventListener('pointerup', this.mouseUp, false)
      }
    },
  })
</script>

<style scoped>
  #review {
    touch-action: manipulation;
  }

  .card {
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background-color: rgba(255, 255, 255, 0.07);
    z-index: 2;
    width: 100%;
    height: 100%;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .btn:hover {
    opacity: 0.75;
  }

  .btn-cyan {
    background-color: #26a69a;
    opacity: 0.87;
  }

  .btn-review-red {
    background-color: #ff7043;
    opacity: 0.87;
  }

  .unselect {
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    -o-user-select: none;
    user-select: none;
    cursor: default;
  }

  #draw-wrapper {
    border: 1px solid rgba(255, 255, 255, 0.6);
    border-radius: 10px;
    width: 100%;
    height: 60vw;
  }

  #canvas-ctrls {
    height: 1rem;
  }

  canvas {
    border-radius: 10px 10px 10px 10px;
    width: 100%;
    height: 87.5%;
    touch-action: none;
  }

  .bi {
    width: 0.75em;
    height: 0.75em;
  }

  #clearbtn:hover {
    opacity: 0.75;
  }

  @media (max-width: 768px) and (orientation: landscape) {
    #canvas-ctrls {
      margin-top: 1.5rem;
    }
  }

  @media (min-width: 768px) and (max-width: 1024px) and (orientation: portrait) {
    #canvas-ctrls {
      margin-top: 2rem;
    }
  }

  @media (min-width: 1024px) {
    .card-container {
      display: flex;
      justify-content: space-between;
    }
    .card {
      height: 40vh;
    }
    #draw-wrapper {
      width: 80vh;
      height: 40vh;
    }
    .bi {
      width: 1.25em;
      height: 1.25em;
    }
  }
</style>

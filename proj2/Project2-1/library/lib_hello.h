/**
 * @file      lib_hello.h
 * @author    Jeramie Vens
 * @date      2015-02-24: Last updated
 * @brief     Library example source
 * @copyright MIT License (c) 2015
 */
 
/*
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*/


#ifndef LIB_HELLO_H
#define LIB_HELLO_H

#ifdef __cplusplus
extern "C" {
#endif

/// Print out hello
void print_hello(void);

/// Print out world num times
int print_world(int num);

#ifdef __cplusplus
}
#endif

#endif

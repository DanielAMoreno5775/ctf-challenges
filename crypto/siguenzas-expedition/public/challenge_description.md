# Password Reset
You're trying to access an old account of yours for a Flash game. Unfortunately, you no longer have access to the email you used so you can't get the confirmation codes. Thankfully, you know something about the game's development. They used the same algorithm as JavaScript's `Math.random()`, but they rolled their own.

You've requested three password resets for another account and email that you can access. As such, you've gotten 3 sequential confirmation codes: 2725572247253476556, 15591863164500473095, and 55102595811394477. Can you obtain the 2 states used so that you can predict the future confirmation codes?

Submit the flag as `CTF{seed1_seed2}` with the seeds being all lowercase. Don't include any prefixes that are used to denote numeric base.

## Hints
* JavaScript's Math.random() function uses the Xorshift128+ algorithm.
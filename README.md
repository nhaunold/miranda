Miranda is a PoC exploit of an XNU security vulnerability that I found in early 2012, intended to be used for a jailbreak at that time but pulled back once we discovered that the kernel extension being exploited wasn't available on one of the devices we wanted to support.

I recently went through an old harddrive and found this code, the beginnings of fooling around and trying to get code to run, which in the end happened. Fun learning exercise and looking back the some of the most fun I've had tinkering around with XNU.

Hazy details, after all it's been ~5 years: When used correctly, you could write data into memory with the help of the R5 register that you had full control over.

In the implementation, we first used this to upload the shellcode, starting at a scratch location for the shellcode and then writing it into memory piece by piece.

Then we'd take advantage of the syscall table in XNU by overwriting the function pointer of the 0 syscall. Using the exact trick as above, we'd first compute the address of syscall 0 and then write the address of the function we wanted to call.
Then, it was only a matter of calling `syscall(0);` to execute that function.

This is used to invalidate and flush the caches and lastly to execute our custom code.

Fun times.

-----

In true iPhone fashion, there's a hash I posted on a Pastebin in 2012.

```sha512("DPTX is actually really cool.")```

— http://pastebin.com/vBN95rGg

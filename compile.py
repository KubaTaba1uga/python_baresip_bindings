import sys
from pathlib import Path


def main():
    args = parse_args()

    baresip_path, re_path = Path(args["baresip_path"]), Path(args["re_path"])

    print(baresip_path)
    print(re_path)


def parse_args():
    if len(sys.argv) < 3:
        print(
            """Usage:
        python3 compile.py <re_path> <baresip_path>
        """
        )
        sys.exit(1)

    return {"re_path": sys.argv[1], "baresip_path": sys.argv[2]}


if __name__ == "__main__":
    main()

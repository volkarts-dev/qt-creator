/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include "timelinemodel.h"
#include <functional>

namespace Timeline {

struct HueLookupTable {
    QRgb table[360];
    HueLookupTable();

    QRgb operator[](int hue) const { return table[hue % 360]; }
};

class TRACING_EXPORT TimelineModel::TimelineModelPrivate {
public:

    static const HueLookupTable hueTable;
    static const int DefaultRowHeight = 30;

    enum BoxColorProperties {
        SelectionIdHueMultiplier = 25,
        FractionHueMultiplier = 96,
        FractionHueMininimum = 10,
        Saturation = 150,
        Lightness = 166
    };

    struct Range {
        Range() : start(-1), duration(-1), selectionId(-1), parent(-1), endIndex(-1) {}
        Range(qint64 start, qint64 duration, int selectionId) :
            start(start), duration(duration), selectionId(selectionId), parent(-1), endIndex(-1) {}
        qint64 start;
        qint64 duration;
        int selectionId;
        int parent;
        int endIndex;
        inline qint64 timestamp() const {return start;}
    };

    struct RangeEnd {
        RangeEnd() : startIndex(-1), end(-1) {}
        RangeEnd(int startIndex, qint64 end) :
            startIndex(startIndex), end(end) {}
        int startIndex;
        qint64 end;
        inline qint64 timestamp() const {return end;}
    };

    TimelineModelPrivate(int modelId);

    int firstIndexNoParents(qint64 startTime) const;

    void incrementStartIndices(int index)
    {
        for (index = index + 1; index < ranges.count(); index++) {
            if (ranges[index].endIndex >= 0)
                endTimes[ranges[index].endIndex].startIndex++;
        }
    }
    void incrementEndIndices(int index)
    {
        for (index = index + 1; index < endTimes.count(); index++)
            ranges[endTimes[index].startIndex].endIndex++;
    }

    inline void setEndIndex(int index, int endIndex)
    {
        ranges[index].endIndex = endIndex;
    }

    inline int insertStart(const Range &start)
    {
        for (int i = ranges.count();;) {
            if (i == 0) {
                ranges.prepend(start);
                return 0;
            }
            const Range &range = ranges.at(--i);
            if (range.start < start.start || (range.start == start.start &&
                                              range.duration >= start.duration)) {
                ranges.insert(++i, start);
                return i;
            }
        }
    }

    inline int insertEnd(const RangeEnd &end)
    {
        for (int i = endTimes.count();;) {
            if (i == 0) {
                endTimes.prepend(end);
                return 0;
            }
            if (endTimes.at(--i).end <= end.end) {
                endTimes.insert(++i, end);
                return i;
            }
        }
    }

    template<typename RangeDelimiter>
    static inline int lowerBound(const QVector<RangeDelimiter> &container, qint64 time)
    {
        int fromIndex = 0;
        int toIndex = container.count() - 1;
        while (toIndex - fromIndex > 1) {
            int midIndex = (fromIndex + toIndex)/2;
            if (container[midIndex].timestamp() < time)
                fromIndex = midIndex;
            else
                toIndex = midIndex;
        }

        return fromIndex;
    }

    int prevItemById(std::function<bool(int)> matchesId, qint64 time, int currentSelected) const;
    int nextItemById(std::function<bool(int)> matchesId, qint64 time, int currentSelected) const;

    QVector<Range> ranges;
    QVector<RangeEnd> endTimes;

    QVector<int> rowOffsets;
    const int modelId;
    QString displayName;
    QString tooltip;
    QColor categoryColor;
    bool hasMixedTypesInExpandedState;

    bool expanded;
    bool hidden;
    int expandedRowCount;
    int collapsedRowCount;
};

} // namespace Timeline
